# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20150604092917) do

  create_table "calendar_dates", force: :cascade do |t|
    t.integer "calendar_id", limit: 8, default: 0, null: false
    t.date    "caledate",                          null: false
  end

  add_index "calendar_dates", ["calendar_id", "caledate"], name: "index_calendar_dates_on_calendar_id_and_caledate", unique: true, using: :btree

  create_table "calendars", force: :cascade do |t|
    t.string   "name",        limit: 255,              null: false
    t.string   "description", limit: 255, default: "", null: false
    t.integer  "kind",        limit: 4,   default: 0,  null: false
    t.datetime "created_at",                           null: false
    t.datetime "updated_at",                           null: false
  end

  add_index "calendars", ["name"], name: "index_calendars_on_name", unique: true, using: :btree

  create_table "clockjobs", force: :cascade do |t|
    t.integer  "jobunit_id",         limit: 8, default: 0, null: false
    t.integer  "basetime",           limit: 4, default: 0, null: false
    t.datetime "predetermined_time"
    t.integer  "day",                limit: 4, default: 0, null: false
    t.integer  "hour",               limit: 4, default: 0, null: false
    t.integer  "minute",             limit: 4, default: 0, null: false
    t.integer  "delay_limit",        limit: 4, default: 1, null: false
    t.integer  "timeover",           limit: 4, default: 0, null: false
  end

  add_index "clockjobs", ["jobunit_id"], name: "index_clockjobs_on_jobunit_id", using: :btree

  create_table "conditions", force: :cascade do |t|
    t.integer "jobunit_id", limit: 8,   default: 0,     null: false
    t.integer "method",     limit: 4,   default: 0,     null: false
    t.integer "kind",       limit: 4,   default: 0,     null: false
    t.string  "cond",       limit: 255, default: "",    null: false
    t.boolean "negative",   limit: 1,   default: false, null: false
  end

  add_index "conditions", ["jobunit_id"], name: "index_conditions_on_jobunit_id", using: :btree

  create_table "configures", force: :cascade do |t|
    t.integer "end_period",         limit: 4, default: 1,  null: false
    t.integer "error_period",       limit: 4, default: 24, null: false
    t.integer "outschedule_period", limit: 4, default: 1,  null: false
    t.integer "carryover_period",   limit: 4, default: 1,  null: false
    t.integer "forcestop_period",   limit: 4, default: 1,  null: false
    t.integer "history_period",     limit: 4, default: 30, null: false
  end

  create_table "connectors", force: :cascade do |t|
    t.integer "jobunit_id", limit: 8, default: 0, null: false
    t.integer "kind",       limit: 4, default: 0, null: false
    t.integer "prev_jobid", limit: 8,             null: false
    t.integer "next_jobid", limit: 8,             null: false
  end

  add_index "connectors", ["jobunit_id"], name: "index_connectors_on_jobunit_id", using: :btree

  create_table "dateconds", force: :cascade do |t|
    t.integer "jobunit_id",  limit: 8, default: 0,     null: false
    t.integer "kind",        limit: 4, default: 0,     null: false
    t.integer "cond",        limit: 4, default: 1,     null: false
    t.integer "calendar_id", limit: 8, default: 0,     null: false
    t.boolean "negative",    limit: 1, default: false, null: false
  end

  add_index "dateconds", ["jobunit_id"], name: "index_dateconds_on_jobunit_id", using: :btree

  create_table "datejobs", force: :cascade do |t|
    t.integer "jobunit_id",        limit: 8, default: 0, null: false
    t.integer "baseday",           limit: 4, default: 0, null: false
    t.integer "beginning_of_week", limit: 4, default: 0, null: false
  end

  add_index "datejobs", ["jobunit_id"], name: "index_datejobs_on_jobunit_id", using: :btree

  create_table "hist_clockjobs", force: :cascade do |t|
    t.integer  "hist_jobunit_id",    limit: 8, default: 0, null: false
    t.integer  "basetime",           limit: 4, default: 0, null: false
    t.datetime "predetermined_time"
    t.integer  "day",                limit: 4, default: 0, null: false
    t.integer  "hour",               limit: 4, default: 0, null: false
    t.integer  "minute",             limit: 4, default: 0, null: false
    t.integer  "delay_limit",        limit: 4, default: 1, null: false
    t.integer  "timeover",           limit: 4, default: 0, null: false
  end

  add_index "hist_clockjobs", ["hist_jobunit_id"], name: "index_hist_clockjobs_on_hist_jobunit_id", using: :btree

  create_table "hist_conditions", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8,   default: 0,     null: false
    t.integer "method",          limit: 4,   default: 0,     null: false
    t.integer "kind",            limit: 4,   default: 0,     null: false
    t.string  "cond",            limit: 255, default: "",    null: false
    t.boolean "negative",        limit: 1,   default: false, null: false
  end

  add_index "hist_conditions", ["hist_jobunit_id"], name: "index_hist_conditions_on_hist_jobunit_id", using: :btree

  create_table "hist_connectors", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8, default: 0, null: false
    t.integer "kind",            limit: 4, default: 0, null: false
    t.integer "prev_jobid",      limit: 8,             null: false
    t.integer "next_jobid",      limit: 8,             null: false
  end

  add_index "hist_connectors", ["hist_jobunit_id"], name: "index_hist_connectors_on_hist_jobunit_id", using: :btree

  create_table "hist_dateconds", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8, default: 0,     null: false
    t.integer "kind",            limit: 4, default: 0,     null: false
    t.integer "cond",            limit: 4, default: 1,     null: false
    t.integer "calendar_id",     limit: 8, default: 0,     null: false
    t.boolean "negative",        limit: 1, default: false, null: false
  end

  add_index "hist_dateconds", ["hist_jobunit_id"], name: "index_hist_dateconds_on_hist_jobunit_id", using: :btree

  create_table "hist_datejobs", force: :cascade do |t|
    t.integer "hist_jobunit_id",   limit: 8, default: 0, null: false
    t.integer "baseday",           limit: 4, default: 0, null: false
    t.integer "beginning_of_week", limit: 4, default: 0, null: false
  end

  add_index "hist_datejobs", ["hist_jobunit_id"], name: "index_hist_datejobs_on_hist_jobunit_id", using: :btree

  create_table "hist_jobresults", force: :cascade do |t|
    t.integer  "hist_jobunit_id", limit: 8,     default: 0,  null: false
    t.integer  "exitcode",        limit: 4,     default: -1, null: false
    t.string   "exitsignal",      limit: 255,   default: "", null: false
    t.text     "stdout",          limit: 65535
    t.text     "stderr",          limit: 65535
    t.datetime "created_at",                                 null: false
    t.datetime "updated_at",                                 null: false
  end

  add_index "hist_jobresults", ["hist_jobunit_id"], name: "index_hist_jobresults_on_hist_jobunit_id", using: :btree

  create_table "hist_jobunits", force: :cascade do |t|
    t.string   "name",          limit: 255,   default: "",    null: false
    t.string   "description",   limit: 255,   default: "",    null: false
    t.integer  "kind",          limit: 4,     default: 0,     null: false
    t.integer  "parent_id",     limit: 8,     default: 0,     null: false
    t.integer  "x",             limit: 4,     default: 0,     null: false
    t.integer  "y",             limit: 4,     default: 0,     null: false
    t.boolean  "hold",          limit: 1,     default: false, null: false
    t.boolean  "skip",          limit: 1,     default: false, null: false
    t.integer  "timeout",       limit: 4,     default: 0,     null: false
    t.datetime "created_at",                                  null: false
    t.datetime "updated_at",                                  null: false
    t.integer  "jobunit_id",    limit: 8,     default: 0,     null: false
    t.integer  "status",        limit: 4,     default: 0,     null: false
    t.datetime "start_time"
    t.datetime "end_time"
    t.boolean  "timeout_occur", limit: 1,     default: false, null: false
    t.integer  "mode",          limit: 4,     default: 0,     null: false
    t.datetime "schedule_time"
    t.text     "path",          limit: 65535
  end

  add_index "hist_jobunits", ["parent_id"], name: "index_hist_jobunits_on_parent_id", using: :btree

  create_table "hist_rootjobnets", force: :cascade do |t|
    t.integer  "hist_jobunit_id", limit: 8, default: 0,     null: false
    t.integer  "user_id",         limit: 8, default: 0,     null: false
    t.boolean  "disabled",        limit: 1, default: false, null: false
    t.datetime "active_since"
    t.datetime "active_till"
    t.integer  "calendar_id",     limit: 8, default: 0,     null: false
    t.integer  "run_type",        limit: 4, default: 0,     null: false
    t.integer  "delay_limit",     limit: 4, default: 1,     null: false
    t.integer  "loader_interval", limit: 4, default: 5,     null: false
  end

  add_index "hist_rootjobnets", ["hist_jobunit_id"], name: "index_hist_rootjobnets_on_hist_jobunit_id", using: :btree

  create_table "hist_schedules", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8,   default: 0,  null: false
    t.string  "week",            limit: 255, default: "", null: false
    t.string  "month",           limit: 255, default: "", null: false
    t.string  "day",             limit: 255, default: "", null: false
    t.string  "hour",            limit: 255, default: "", null: false
    t.string  "minute",          limit: 255, default: "", null: false
  end

  add_index "hist_schedules", ["hist_jobunit_id"], name: "index_hist_schedules_on_hist_jobunit_id", using: :btree

  create_table "hist_sleepjobs", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8, default: 0, null: false
    t.integer "hour",            limit: 4, default: 0, null: false
    t.integer "minute",          limit: 4, default: 0, null: false
    t.integer "second",          limit: 4, default: 0, null: false
  end

  add_index "hist_sleepjobs", ["hist_jobunit_id"], name: "index_hist_sleepjobs_on_hist_jobunit_id", using: :btree

  create_table "hist_sshjobs", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8,     default: 0,  null: false
    t.integer "authtype",        limit: 4,     default: 0,  null: false
    t.string  "host",            limit: 255,   default: "", null: false
    t.string  "user",            limit: 255,   default: "", null: false
    t.string  "password",        limit: 255,   default: "", null: false
    t.integer "port",            limit: 4,     default: 0,  null: false
    t.text    "privatekey",      limit: 65535
    t.text    "command",         limit: 65535
  end

  add_index "hist_sshjobs", ["hist_jobunit_id"], name: "index_hist_sshjobs_on_hist_jobunit_id", using: :btree

  create_table "hist_vardata", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8,     default: 0,    null: false
    t.string  "name",            limit: 255,   default: "",   null: false
    t.text    "value",           limit: 65535
    t.boolean "overwrite",       limit: 1,     default: true, null: false
  end

  add_index "hist_vardata", ["hist_jobunit_id"], name: "index_hist_vardata_on_hist_jobunit_id", using: :btree

  create_table "hist_variables", force: :cascade do |t|
    t.integer  "hist_jobunit_id", limit: 8,     default: 0,  null: false
    t.string   "name",            limit: 255,   default: "", null: false
    t.text     "value",           limit: 65535
    t.datetime "created_at",                                 null: false
    t.datetime "updated_at",                                 null: false
  end

  add_index "hist_variables", ["hist_jobunit_id"], name: "index_hist_variables_on_hist_jobunit_id", using: :btree

  create_table "hist_winjobs", force: :cascade do |t|
    t.integer "hist_jobunit_id", limit: 8,     default: 0,   null: false
    t.string  "host",            limit: 255,   default: "",  null: false
    t.string  "user",            limit: 255,   default: "",  null: false
    t.string  "password",        limit: 255,   default: "",  null: false
    t.string  "scheme",          limit: 255,   default: "",  null: false
    t.integer "port",            limit: 4,     default: 0,   null: false
    t.string  "path",            limit: 255,   default: "",  null: false
    t.string  "auth",            limit: 255,   default: "",  null: false
    t.integer "codepage",        limit: 4,     default: 932, null: false
    t.text    "command",         limit: 65535
  end

  add_index "hist_winjobs", ["hist_jobunit_id"], name: "index_hist_winjobs_on_hist_jobunit_id", using: :btree

  create_table "jobunits", force: :cascade do |t|
    t.string   "name",        limit: 255,                 null: false
    t.string   "description", limit: 255, default: "",    null: false
    t.integer  "kind",        limit: 4,   default: 0,     null: false
    t.integer  "parent_id",   limit: 8,   default: 0,     null: false
    t.integer  "x",           limit: 4,   default: 0,     null: false
    t.integer  "y",           limit: 4,   default: 0,     null: false
    t.boolean  "hold",        limit: 1,   default: false, null: false
    t.boolean  "skip",        limit: 1,   default: false, null: false
    t.integer  "timeout",     limit: 4,   default: 0,     null: false
    t.datetime "created_at",                              null: false
    t.datetime "updated_at",                              null: false
  end

  add_index "jobunits", ["kind"], name: "index_jobunits_on_kind", using: :btree
  add_index "jobunits", ["name", "parent_id"], name: "index_jobunits_on_name_and_parent_id", unique: true, using: :btree

  create_table "proc_clockjobs", force: :cascade do |t|
    t.integer  "proc_jobunit_id",    limit: 8, default: 0, null: false
    t.integer  "basetime",           limit: 4, default: 0, null: false
    t.datetime "predetermined_time"
    t.integer  "day",                limit: 4, default: 0, null: false
    t.integer  "hour",               limit: 4, default: 0, null: false
    t.integer  "minute",             limit: 4, default: 0, null: false
    t.integer  "timeover",           limit: 4, default: 0, null: false
    t.integer  "delay_limit",        limit: 4, default: 1, null: false
  end

  add_index "proc_clockjobs", ["proc_jobunit_id"], name: "index_proc_clockjobs_on_proc_jobunit_id", using: :btree

  create_table "proc_conditions", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8,   default: 0,     null: false
    t.integer "method",          limit: 4,   default: 0,     null: false
    t.integer "kind",            limit: 4,   default: 0,     null: false
    t.string  "cond",            limit: 255, default: "",    null: false
    t.boolean "negative",        limit: 1,   default: false, null: false
  end

  add_index "proc_conditions", ["proc_jobunit_id"], name: "index_proc_conditions_on_proc_jobunit_id", using: :btree

  create_table "proc_connectors", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8, default: 0, null: false
    t.integer "kind",            limit: 4, default: 0, null: false
    t.integer "prev_jobid",      limit: 8,             null: false
    t.integer "next_jobid",      limit: 8,             null: false
  end

  add_index "proc_connectors", ["prev_jobid"], name: "index_proc_connectors_on_prev_jobid", using: :btree
  add_index "proc_connectors", ["proc_jobunit_id"], name: "index_proc_connectors_on_proc_jobunit_id", using: :btree

  create_table "proc_dateconds", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8, default: 0,     null: false
    t.integer "kind",            limit: 4, default: 0,     null: false
    t.integer "cond",            limit: 4, default: 1,     null: false
    t.integer "calendar_id",     limit: 8, default: 0,     null: false
    t.boolean "negative",        limit: 1, default: false, null: false
  end

  add_index "proc_dateconds", ["proc_jobunit_id"], name: "index_proc_dateconds_on_proc_jobunit_id", using: :btree

  create_table "proc_datejobs", force: :cascade do |t|
    t.integer "proc_jobunit_id",   limit: 8, default: 0, null: false
    t.integer "baseday",           limit: 4, default: 0, null: false
    t.integer "beginning_of_week", limit: 4, default: 0, null: false
  end

  add_index "proc_datejobs", ["proc_jobunit_id"], name: "index_proc_datejobs_on_proc_jobunit_id", using: :btree

  create_table "proc_execlogs", force: :cascade do |t|
    t.integer  "proc_jobunit_id", limit: 8,     default: 0, null: false
    t.integer  "kind",            limit: 4,     default: 0, null: false
    t.integer  "code",            limit: 4,     default: 0, null: false
    t.text     "message",         limit: 65535
    t.datetime "created_at",                                null: false
    t.datetime "updated_at",                                null: false
  end

  add_index "proc_execlogs", ["proc_jobunit_id"], name: "index_proc_execlogs_on_proc_jobunit_id", using: :btree

  create_table "proc_jobresults", force: :cascade do |t|
    t.integer  "proc_jobunit_id", limit: 8,     default: 0,  null: false
    t.integer  "exitcode",        limit: 4,     default: -1, null: false
    t.string   "exitsignal",      limit: 255,   default: "", null: false
    t.text     "stdout",          limit: 65535
    t.text     "stderr",          limit: 65535
    t.datetime "created_at",                                 null: false
    t.datetime "updated_at",                                 null: false
  end

  add_index "proc_jobresults", ["proc_jobunit_id"], name: "index_proc_jobresults_on_proc_jobunit_id", using: :btree

  create_table "proc_jobunits", force: :cascade do |t|
    t.string   "name",          limit: 255,   default: "",    null: false
    t.string   "description",   limit: 255,   default: "",    null: false
    t.integer  "kind",          limit: 4,     default: 0,     null: false
    t.integer  "parent_id",     limit: 8,     default: 0,     null: false
    t.integer  "x",             limit: 4,     default: 0,     null: false
    t.integer  "y",             limit: 4,     default: 0,     null: false
    t.boolean  "hold",          limit: 1,     default: false, null: false
    t.boolean  "skip",          limit: 1,     default: false, null: false
    t.integer  "timeout",       limit: 4,     default: 0,     null: false
    t.datetime "created_at",                                  null: false
    t.datetime "updated_at",                                  null: false
    t.integer  "jobunit_id",    limit: 8,     default: 0,     null: false
    t.integer  "status",        limit: 4,     default: 0,     null: false
    t.datetime "start_time"
    t.datetime "end_time"
    t.boolean  "timeout_occur", limit: 1,     default: false, null: false
    t.boolean  "force_start",   limit: 1,     default: false, null: false
    t.boolean  "force_stop",    limit: 1,     default: false, null: false
    t.integer  "boot",          limit: 4,     default: 0,     null: false
    t.integer  "finish",        limit: 4,     default: 0,     null: false
    t.integer  "pid",           limit: 4,     default: 0,     null: false
    t.text     "path",          limit: 65535
  end

  add_index "proc_jobunits", ["parent_id"], name: "index_proc_jobunits_on_parent_id", using: :btree
  add_index "proc_jobunits", ["status"], name: "index_proc_jobunits_on_status", using: :btree

  create_table "proc_rootjobnets", force: :cascade do |t|
    t.integer  "proc_jobunit_id", limit: 8, default: 0,     null: false
    t.integer  "user_id",         limit: 8, default: 0,     null: false
    t.boolean  "disabled",        limit: 1, default: false, null: false
    t.datetime "active_since"
    t.datetime "active_till"
    t.integer  "calendar_id",     limit: 8, default: 0,     null: false
    t.integer  "run_type",        limit: 4, default: 0,     null: false
    t.integer  "delay_limit",     limit: 4, default: 1,     null: false
    t.integer  "loader_interval", limit: 4, default: 5,     null: false
  end

  add_index "proc_rootjobnets", ["proc_jobunit_id"], name: "index_proc_rootjobnets_on_proc_jobunit_id", using: :btree

  create_table "proc_schedules", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8,   default: 0,  null: false
    t.string  "week",            limit: 255, default: "", null: false
    t.string  "month",           limit: 255, default: "", null: false
    t.string  "day",             limit: 255, default: "", null: false
    t.string  "hour",            limit: 255, default: "", null: false
    t.string  "minute",          limit: 255, default: "", null: false
  end

  add_index "proc_schedules", ["proc_jobunit_id"], name: "index_proc_schedules_on_proc_jobunit_id", using: :btree

  create_table "proc_sleepjobs", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8, default: 0, null: false
    t.integer "hour",            limit: 4, default: 0, null: false
    t.integer "minute",          limit: 4, default: 0, null: false
    t.integer "second",          limit: 4, default: 0, null: false
  end

  add_index "proc_sleepjobs", ["proc_jobunit_id"], name: "index_proc_sleepjobs_on_proc_jobunit_id", using: :btree

  create_table "proc_sshjobs", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8,     default: 0,  null: false
    t.integer "authtype",        limit: 4,     default: 0,  null: false
    t.string  "host",            limit: 255,   default: "", null: false
    t.string  "user",            limit: 255,   default: "", null: false
    t.string  "password",        limit: 255,   default: "", null: false
    t.integer "port",            limit: 4,     default: 0,  null: false
    t.text    "privatekey",      limit: 65535
    t.text    "command",         limit: 65535
  end

  add_index "proc_sshjobs", ["proc_jobunit_id"], name: "index_proc_sshjobs_on_proc_jobunit_id", using: :btree

  create_table "proc_topjobnets", force: :cascade do |t|
    t.string   "name",            limit: 255,   default: "",    null: false
    t.string   "description",     limit: 255,   default: "",    null: false
    t.integer  "kind",            limit: 4,     default: 0,     null: false
    t.boolean  "hold",            limit: 1,     default: false, null: false
    t.boolean  "skip",            limit: 1,     default: false, null: false
    t.integer  "timeout",         limit: 4,     default: 0,     null: false
    t.datetime "created_at",                                    null: false
    t.datetime "updated_at",                                    null: false
    t.integer  "jobunit_id",      limit: 8,     default: 0,     null: false
    t.integer  "status",          limit: 4,     default: 0,     null: false
    t.datetime "start_time"
    t.datetime "end_time"
    t.boolean  "timeout_occur",   limit: 1,     default: false, null: false
    t.boolean  "force_start",     limit: 1,     default: false, null: false
    t.boolean  "force_stop",      limit: 1,     default: false, null: false
    t.integer  "proc_jobunit_id", limit: 8,     default: 0,     null: false
    t.integer  "mode",            limit: 4,     default: 0,     null: false
    t.datetime "schedule_time"
    t.integer  "run_type",        limit: 4,     default: 0,     null: false
    t.integer  "delay_limit",     limit: 4,     default: 1,     null: false
    t.text     "path",            limit: 65535
  end

  add_index "proc_topjobnets", ["jobunit_id"], name: "index_proc_topjobnets_on_jobunit_id", using: :btree
  add_index "proc_topjobnets", ["proc_jobunit_id"], name: "index_proc_topjobnets_on_proc_jobunit_id", using: :btree
  add_index "proc_topjobnets", ["status"], name: "index_proc_topjobnets_on_status", using: :btree

  create_table "proc_vardata", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8,     default: 0,    null: false
    t.string  "name",            limit: 255,   default: "",   null: false
    t.text    "value",           limit: 65535
    t.boolean "overwrite",       limit: 1,     default: true, null: false
  end

  add_index "proc_vardata", ["proc_jobunit_id"], name: "index_proc_vardata_on_proc_jobunit_id", using: :btree

  create_table "proc_variables", force: :cascade do |t|
    t.integer  "proc_jobunit_id", limit: 8,     default: 0,  null: false
    t.string   "name",            limit: 255,   default: "", null: false
    t.text     "value",           limit: 65535
    t.datetime "created_at",                                 null: false
    t.datetime "updated_at",                                 null: false
  end

  add_index "proc_variables", ["proc_jobunit_id"], name: "index_proc_variables_on_proc_jobunit_id", using: :btree

  create_table "proc_winjobs", force: :cascade do |t|
    t.integer "proc_jobunit_id", limit: 8,     default: 0,   null: false
    t.string  "host",            limit: 255,   default: "",  null: false
    t.string  "user",            limit: 255,   default: "",  null: false
    t.string  "password",        limit: 255,   default: "",  null: false
    t.string  "scheme",          limit: 255,   default: "",  null: false
    t.integer "port",            limit: 4,     default: 0,   null: false
    t.string  "path",            limit: 255,   default: "",  null: false
    t.string  "auth",            limit: 255,   default: "",  null: false
    t.integer "codepage",        limit: 4,     default: 932, null: false
    t.text    "command",         limit: 65535
  end

  add_index "proc_winjobs", ["proc_jobunit_id"], name: "index_proc_winjobs_on_proc_jobunit_id", using: :btree

  create_table "rootjobnets", force: :cascade do |t|
    t.integer  "jobunit_id",      limit: 8, default: 0,     null: false
    t.integer  "user_id",         limit: 8, default: 0,     null: false
    t.boolean  "disabled",        limit: 1, default: false, null: false
    t.datetime "active_since"
    t.datetime "active_till"
    t.integer  "calendar_id",     limit: 8, default: 0,     null: false
    t.integer  "run_type",        limit: 4, default: 0,     null: false
    t.integer  "delay_limit",     limit: 4, default: 1,     null: false
    t.integer  "loader_interval", limit: 4, default: 5,     null: false
  end

  add_index "rootjobnets", ["jobunit_id"], name: "index_rootjobnets_on_jobunit_id", using: :btree
  add_index "rootjobnets", ["user_id"], name: "index_rootjobnets_on_user_id", using: :btree

  create_table "schedules", force: :cascade do |t|
    t.integer "jobunit_id", limit: 8,   default: 0,  null: false
    t.string  "week",       limit: 255, default: "", null: false
    t.string  "month",      limit: 255, default: "", null: false
    t.string  "day",        limit: 255, default: "", null: false
    t.string  "hour",       limit: 255, default: "", null: false
    t.string  "minute",     limit: 255, default: "", null: false
  end

  add_index "schedules", ["jobunit_id"], name: "index_schedules_on_jobunit_id", using: :btree

  create_table "sleepjobs", force: :cascade do |t|
    t.integer "jobunit_id", limit: 8, default: 0, null: false
    t.integer "hour",       limit: 4, default: 0, null: false
    t.integer "minute",     limit: 4, default: 0, null: false
    t.integer "second",     limit: 4, default: 0, null: false
  end

  add_index "sleepjobs", ["jobunit_id"], name: "index_sleepjobs_on_jobunit_id", using: :btree

  create_table "sshjobs", force: :cascade do |t|
    t.integer "jobunit_id", limit: 8,     default: 0,  null: false
    t.integer "authtype",   limit: 4,     default: 0,  null: false
    t.string  "host",       limit: 255,   default: "", null: false
    t.string  "user",       limit: 255,   default: "", null: false
    t.string  "password",   limit: 255,   default: "", null: false
    t.integer "port",       limit: 4,     default: 22, null: false
    t.text    "privatekey", limit: 65535
    t.text    "command",    limit: 65535
  end

  add_index "sshjobs", ["jobunit_id"], name: "index_sshjobs_on_jobunit_id", using: :btree

  create_table "users", force: :cascade do |t|
    t.string   "name",            limit: 255,                   null: false
    t.string   "fullname",        limit: 255, default: "",      null: false
    t.integer  "kind",            limit: 4,   default: 0,       null: false
    t.string   "password_digest", limit: 255, default: "",      null: false
    t.string   "remember_token",  limit: 255, default: "",      null: false
    t.string   "email",           limit: 255, default: "",      null: false
    t.string   "language",        limit: 255, default: "en_GB", null: false
    t.datetime "created_at",                                    null: false
    t.datetime "updated_at",                                    null: false
  end

  add_index "users", ["name"], name: "index_users_on_name", unique: true, using: :btree
  add_index "users", ["remember_token"], name: "index_users_on_remember_token", using: :btree

  create_table "vardata", force: :cascade do |t|
    t.integer "jobunit_id", limit: 8,     default: 0,    null: false
    t.string  "name",       limit: 255,   default: "",   null: false
    t.text    "value",      limit: 65535
    t.boolean "overwrite",  limit: 1,     default: true, null: false
  end

  add_index "vardata", ["jobunit_id"], name: "index_vardata_on_jobunit_id", using: :btree

  create_table "winjobs", force: :cascade do |t|
    t.integer "jobunit_id", limit: 8,     default: 0,        null: false
    t.string  "host",       limit: 255,   default: "",       null: false
    t.string  "user",       limit: 255,   default: "",       null: false
    t.string  "password",   limit: 255,   default: "",       null: false
    t.string  "scheme",     limit: 255,   default: "http",   null: false
    t.integer "port",       limit: 4,     default: 5985,     null: false
    t.string  "path",       limit: 255,   default: "/wsman", null: false
    t.string  "auth",       limit: 255,   default: "basic",  null: false
    t.integer "codepage",   limit: 4,     default: 932,      null: false
    t.text    "command",    limit: 65535
  end

  add_index "winjobs", ["jobunit_id"], name: "index_winjobs_on_jobunit_id", using: :btree

end
