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
/* These methods will be implemented to reflect the application behavior */

#include "MW.h"
#include "Driver_md5crack.h"
#include "Worker_md5crack.h"
#include "Task_md5crack.h"
#include <unistd.h>
#include "md5.h"
#include <iostream>
using namespace std;

Driver_md5crack::Driver_md5crack()
{
  num_tasks = 0;
  passwd_size = 0;
  passwd_minchar= 0;
  passwd_maxchar = 0;
  passwd = NULL;
}

Driver_md5crack::~Driver_md5crack()
{
  if(passwd != NULL) {
      delete[] passwd;
  }
}

MWReturn Driver_md5crack::get_userinfo( int argc, char *argv[] )
{
  int i, j;
  int num_exec = 0;
  int num_arch = 0;
  char exec[_POSIX_PATH_MAX];

  MWprintf(30, "Enter Driver_md5crack::get_userinfo\n");
  for ( i=0; i<argc; i++ ) {
      MWprintf( 70, "arg %d: %s\n", i, argv[i] );
  }

  RMC->set_num_exec_classes(1);

  scanf ("%d", &num_arch);
  RMC->set_num_arch_classes(num_arch);
  MWprintf( 10, "Set the arch class to %d.\n", num_arch);

  for ( i=0; i<num_arch; i++) {
      if (i==0)
        RMC->set_arch_class_attributes (0, "((Arch==\"x86_64\") && (Opsys==\"LINUX\"))");
      else RMC->set_arch_class_attributes (1, "((Arch==\"INTEL\") && (Opsys==\"SOLARIS26\"))");
  }

  scanf ("%d", &num_exec);
  RMC->set_num_executables(num_exec);
  for ( i=0; i<num_exec; i++) {
      scanf("%s %d", exec, &j);
      MWprintf( 30, " %s\n", exec);
      RMC->add_executable(0, j, exec, "");
  }

  set_checkpoint_frequency (10);

  fgetc(stdin);
  fgets(hash, 33, stdin);
  scanf("%d", &passwd_size);
  scanf("%d", &passwd_minchar);
  scanf("%d", &passwd_maxchar);
  MWprintf(30, "The passwd size is %d.\n", passwd_size);
  MWprintf(30, "The hash is %s.\n", hash);
  MWprintf(30, "The min char is %d.\n", passwd_minchar);
  MWprintf(30, "The max char is %d.\n", passwd_maxchar);

  if (passwd_size <= 0 || passwd_maxchar == 0 ||
      passwd_minchar >= passwd_maxchar) {
      MWprintf(10, "The job size is 0 so nothing to do!\n");
      return QUIT;
  }
  passwd = new char[passwd_size + 1]();
  num_tasks = passwd_maxchar - passwd_minchar + 1;

  RMC->set_target_num_workers(num_tasks);
  MWprintf(30, "Patitioned into %d tasks\n", num_tasks);

  MWprintf(30, "Leave Driver_md5crack::get_userinfo\n");
  return OK;
}

MWReturn Driver_md5crack::setup_initial_tasks(int *n_init , MWTask ***init_tasks)
{
  int i;
  MWprintf(30, "Setting up tasks.\n");

  *n_init = num_tasks;
  *init_tasks = new MWTask *[num_tasks];

  for (i = 0; i < num_tasks; i++) {
      (*init_tasks)[i] = new Task_md5crack(passwd_size, passwd_minchar+i, passwd_minchar);
  }

  return OK;
}

MWReturn Driver_md5crack::act_on_completed_task( MWTask *t )
{
  Task_md5crack *tf = dynamic_cast<Task_md5crack *>(t);
  string s = md5(tf->passwd);
  if(strcmp(hash, s.c_str()) == 0) {
    strncpy(passwd, tf->passwd, passwd_size + 1);
    MWprintf(30, "Driver_md5crack::act_on_completed_task: password found = '%s'.\n", passwd);
    FILE *out = NULL;
    out = fopen("correct_password", "wt");
    if(out == NULL) {
        perror("Error opening file");
    }
    fprintf(out, "%s\n", tf->passwd);
    fclose(out);
  } else {
    MWprintf(30, "Driver_md5crack::act_on_completed_task: password not found.\n");
  }
  return OK;
}

MWReturn Driver_md5crack::pack_worker_init_data( void )
{
  MWprintf(30, "Sending hash='%s', minchar=%d, maxchar=%d to workers.\n", hash, passwd_minchar, passwd_maxchar);
  RMC->pack(&passwd_minchar, 1, 1);
  RMC->pack(&passwd_maxchar, 1, 1);
  RMC->pack(hash, 33, 1);
  return OK;
}

void Driver_md5crack::printresults()
{
  MWprintf ( 10, "The password is %s.\n", passwd);
}

void
Driver_md5crack::write_master_state( FILE *fp )
{
}

void
Driver_md5crack::read_master_state( FILE *fp )
{
}

MWTask*
Driver_md5crack::gimme_a_task()
{
  return new Task_md5crack;
}

MWDriver* gimme_the_master()
{
  return new Driver_md5crack;
}
