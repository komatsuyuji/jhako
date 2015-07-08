/////////////////////////////////////////////////////////////////////////////////
//
// jHako
// Copyright (C) 2014-2015 Komatsu Yuji(Zheng Chuyu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////

#define JOBUNIT_KIND_JOBGROUP 0
#define JOBUNIT_KIND_ROOTJOBNET 10
#define JOBUNIT_KIND_JOBNET 20
#define JOBUNIT_KIND_STARTJOB 100
#define JOBUNIT_KIND_ENDJOB 101
#define JOBUNIT_KIND_MERGEJOB 102
#define JOBUNIT_KIND_SLEEPJOB 103
#define JOBUNIT_KIND_CLOCKJOB 104
#define JOBUNIT_KIND_DATEJOB 105
#define JOBUNIT_KIND_VARJOB 106
#define JOBUNIT_KIND_SSHJOB 200
#define JOBUNIT_KIND_WINJOB 201

#define JOBUNIT_STATUS_NONE 0
#define JOBUNIT_STATUS_STANDBY 1
#define JOBUNIT_STATUS_READY 2
#define JOBUNIT_STATUS_RUNNING 3
#define JOBUNIT_STATUS_END 4
#define JOBUNIT_STATUS_ERROR 5
#define JOBUNIT_STATUS_OUTSCHEDULE 6
#define JOBUNIT_STATUS_CARRYOVER 7
#define JOBUNIT_STATUS_FORCESTOP 8

#define ROOTJOBNET_RUNTYPE_EXCLUSIVE 0
#define ROOTJOBNET_RUNTYPE_FULLEXCLUSIVE 1
#define ROOTJOBNET_RUNTYPE_PARALLEL 2

#define TOPJOBNET_MODE_SCHEDULE 0
#define TOPJOBNET_MODE_IMMEDIATE 1
#define TOPJOBNET_MODE_SPECIFIED 2

#define CONNECTOR_KIND_NORMAL 0
#define CONNECTOR_KIND_BRANCH 1
#define CONNECTOR_KIND_ERROR 2

#define CLOCKJOB_BASETIME_JOB_STARTDAY 0
#define CLOCKJOB_BASETIME_JOB_STARTTIME 1
#define CLOCKJOB_BASETIME_JOBNET_STARTDAY 2
#define CLOCKJOB_BASETIME_JOBNET_STARTTIME 3
#define CLOCKJOB_BASETIME_ROOTJOBNET_STARTDAY 4
#define CLOCKJOB_BASETIME_ROOTJOBNET_STARTTIME 5

#define CLOCKJOB_TIMEOVER_NORMAL 0
#define CLOCKJOB_TIMEOVER_BRANCH 1
#define CLOCKJOB_TIMEOVER_ERROR 2

#define DATEJOB_BASEDAY_JOB 0
#define DATEJOB_BASEDAY_JOBNET 1
#define DATEJOB_BASEDAY_ROOTJOBNET 2

#define DATECOND_KIND_EVERY_WEEK 0
#define DATECOND_KIND_FIRST_WEEK 1
#define DATECOND_KIND_SECOND_WEEK 2
#define DATECOND_KIND_THIRD_WEEK 3
#define DATECOND_KIND_FOURTH_WEEK 4
#define DATECOND_KIND_FIFTH_WEEK 5
#define DATECOND_KIND_EVERY_MONTH 6
#define DATECOND_KIND_REVERSE_MONTH 7
#define DATECOND_KIND_CALENDAR 100

#define VARDATUM_OVERWRITE_FALSE 0
#define VARDATUM_OVERWRITE_TRUE 1

#define CONDITION_METHOD_ERROR 0
#define CONDITION_METHOD_BRANCH 1

#define CONDITION_KIND_EXITCODE 0
#define CONDITION_KIND_EXITSIGNAL 1
#define CONDITION_KIND_STDOUT 2
#define CONDITION_KIND_STDERR 3

#define EXECLOG_KIND_INFO 0
#define EXECLOG_KIND_WARN 1
#define EXECLOG_KIND_ERROR 2

#include <dbi/dbi.h>

#include "jobunit.h"
#include "rootjobnet.h"
#include "schedule.h"
#include "connector.h"
#include "variable.h"
#include "sleepjob.h"
#include "clockjob.h"
#include "datejob.h"
#include "datecond.h"
#include "vardatum.h"
#include "sshjob.h"
#include "winjob.h"
#include "condition.h"
#include "jobresult.h"
#include "execlog.h"
#include "jhkjoblog.h"
#include "calendar.h"
#include "calendar_date.h"
#include "configure.h"
