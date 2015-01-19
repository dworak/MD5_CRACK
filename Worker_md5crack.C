/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 *
 * Condor Software Copyright Notice
 * Copyright (C) 1990-2004, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 *
 * This source code is covered by the Condor Public License, which can
 * be found in the accompanying LICENSE.TXT file, or online at
 * www.condorproject.org.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
 * FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
 * MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
 * ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
 * PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
 * RIGHT.
 *
 ****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/
#include "Worker_md5crack.h"
#include "Task_md5crack.h"
#include "md5.h"
#include <string>
using namespace std;

Worker_md5crack::Worker_md5crack()
{
  workingTask = new Task_md5crack;
  hash = new char[33]();
}

Worker_md5crack::~Worker_md5crack()
{
  delete workingTask;
  delete[] hash;
}

double
Worker_md5crack::benchmark( MWTask *t )
{
  Task_md5crack *tl = dynamic_cast<Task_md5crack *> ( t );
  tl->printself(30);
  return 3.14159;
}

MWReturn Worker_md5crack::unpack_init_data( void )
{
  RMC->unpack(&passwd_minchar, 1, 1);
  RMC->unpack(&passwd_maxchar, 1, 1);
  RMC->unpack(hash, 33, 1);
  MWprintf(30, "Recived initial hash=%s, minchar=%d, maxchar=%d.\n", hash, passwd_minchar, passwd_maxchar);
  return OK;
}

void Worker_md5crack::execute_task( MWTask *t )
{
  MWprintf(30, "Enter Worker_md5crack::execute_task\n");
  Task_md5crack *tl = dynamic_cast<Task_md5crack *> ( t );
  MWprintf(30, "The task I am working on is: %s\n", tl->passwd);
  generate_and_check(tl->passwd, tl->passwd_size, 1);
  MWprintf(30, "Leave Worker_md5crack::execute_task for passwd:%s\n", tl->passwd);
}

bool Worker_md5crack::generate_and_check(char *passwd, int size, int pos) {
    if(pos < size - 1) {
        for(int i = passwd_minchar; i <= passwd_maxchar; i++) {
            passwd[pos] = (char) i;
            if(generate_and_check(passwd, size, pos+1) == true){
                return true;
            }
        }
    } else {
        string s;
        for(int i = passwd_minchar; i <= passwd_maxchar; i++) {
            passwd[pos] = (char) i;
            s = md5(passwd);
            if(strcmp(hash, s.c_str()) == 0) {
                return true;
            }
        }
        return false;
    }

    return false;
}

MWTask* Worker_md5crack::gimme_a_task()
{
  return new Task_md5crack;
}

MWWorker*
gimme_a_worker ()
{
  return new Worker_md5crack;
}
