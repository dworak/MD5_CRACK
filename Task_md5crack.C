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
#include "Task_md5crack.h"
#include "MW.h"

Task_md5crack::Task_md5crack()
{
  passwd = NULL;
  passwd_size = 0;
}

Task_md5crack::Task_md5crack(int passwd_size, int first_letter, int passwd_minchar)
{
  if(passwd_size <= 0) {
      MWprintf(30, "Task construction: array size <= 0! \n");
  }
  this->passwd_size = passwd_size;
  passwd = new char[passwd_size + 1]();
  passwd[0] = (char) first_letter;
  for(int i = 1; i < passwd_size; i++) {
    passwd[i] = (char) passwd_minchar;
  }
  printself(30);
}

Task_md5crack::~Task_md5crack() {
    delete[] passwd;
}

void Task_md5crack::printself( int level )
{
  MWprintf(level, "Task for passwd=%s\n", passwd);
}

void Task_md5crack::pack_work( void )
{
  RMC->pack(&passwd_size, 1, 1);
  RMC->pack(passwd, passwd_size + 1, 1);
}

void Task_md5crack::unpack_work( void )
{
  RMC->unpack(&passwd_size, 1, 1);
  if(passwd != NULL) {
      delete[] passwd;
  }
  passwd = new char[passwd_size + 1]();
  RMC->unpack(passwd, passwd_size + 1, 1);
}

void Task_md5crack::pack_results( void )
{
  RMC->pack(&passwd_size, 1, 1);
  RMC->pack(passwd, passwd_size + 1, 1);
}

void Task_md5crack::unpack_results( void )
{
  RMC->unpack(&passwd_size, 1, 1);
  if(passwd != NULL) {
      delete[] passwd;
  }
  passwd = new char[passwd_size + 1]();
  RMC->unpack(passwd, passwd_size + 1, 1);
}

void Task_md5crack::write_ckpt_info( FILE *fp )
{
}

void Task_md5crack::read_ckpt_info( FILE *fp )
{
}
