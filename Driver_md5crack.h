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
#ifndef _md5crack_DRIVER_H
#define _md5crack_DRIVER_H

#include "MWDriver.h"
#include "Task_md5crack.h"
#include <iostream>

class Driver_md5crack : public MWDriver
{
public:
       	Driver_md5crack();
       	~Driver_md5crack();
       	MWReturn get_userinfo( int argc, char *argv[] );
       	MWReturn setup_initial_tasks( int *, MWTask *** );
       	MWReturn act_on_completed_task( MWTask * );
       	MWReturn pack_worker_init_data( void );
       	void printresults();
       	void write_master_state( FILE *fp );
       	void read_master_state( FILE *fp );
       	MWTask* gimme_a_task();

private:
        char hash[33];
        char *passwd;
        int passwd_minchar;
        int passwd_maxchar;
        int passwd_size;
       	int num_tasks;
};

#endif
