

{.test_func = gzis_sem_test_010, .test_name="gzis_sem_test_010", .test_desc="Initialize sem with invalid values(-1,0)", .expected_error=ERR_OUTOF_RANGE, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_011, .test_name="gzis_sem_test_011", .test_desc="Initialize sem with invalid values(-1,-1)", .expected_error=ERR_OUTOF_RANGE, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_012, .test_name="gzis_sem_test_012", .test_desc="Initialize sem with invalid values(0,-1)", .expected_error=ERR_OUTOF_RANGE, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_013, .test_name="gzis_sem_test_013", .test_desc="Initialize sem in isr with valid params", .expected_error=ERR_NO_SUPPORT, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_014, .test_name="gzis_sem_test_014", .test_desc="Initialize sem in isr with invalid params(sem_id=NULL)", .expected_error=ERR_NO_SUPPORT, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_015, .test_name="gzis_sem_test_015", .test_desc="Initialize sem in isr with invalid params(-1,0)", .expected_error=ERR_NO_SUPPORT, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_016, .test_name="gzis_sem_test_016", .test_desc="Initialize sem in isr with invalid params(0,-1)", .expected_error=ERR_NO_SUPPORT, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_017, .test_name="gzis_sem_test_017", .test_desc="Initialize sem in isr with invalid params(-1,-1)", .expected_error=ERR_NO_SUPPORT, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_005, .test_name="gzis_sem_test_005", .test_desc="Initialize sem with valid values(0,0)", .expected_error=ERR_NO_ERROR, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_006, .test_name="gzis_sem_test_006", .test_desc="Initialize sem with valid values(0,1)", .expected_error=ERR_NO_ERROR, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_007, .test_name="gzis_sem_test_007", .test_desc="Initialize sem with valid values(1,1)", .expected_error=ERR_NO_ERROR, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_008, .test_name="gzis_sem_test_008", .test_desc="Initialize sem with valid values(0,GZIS_SEM_VALUE_MAX)", .expected_error=ERR_NO_ERROR, .real_error=ERR_NO_INITIALIZED},
{.test_func = gzis_sem_test_009, .test_name="gzis_sem_test_009", .test_desc="Initialize sem with valid values(GZIS_SEM_VALUE_MAX,GZIS_SEM_VALUE_MAX)", .expected_error=ERR_NO_ERROR, .real_error=ERR_NO_INITIALIZED},


void gzis_sem_test_005();
void gzis_sem_test_006();
void gzis_sem_test_007();
void gzis_sem_test_008();
void gzis_sem_test_009();
void gzis_sem_test_010();
void gzis_sem_test_011();
void gzis_sem_test_012();
void gzis_sem_test_013();
void gzis_sem_test_014();
void gzis_sem_test_015();
void gzis_sem_test_016();
void gzis_sem_test_017();

/**
 * @brief Initialize sem with invalid values(-1,0)
 * 
 * sem_id = valid
 * initial_value = -1
 * max_value = 0
 * context = task
 */
void gzis_sem_test_010()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, -1, 0);
    STF_CHECK_RETURN_TEST(ret);
}

/**
 * @brief Initialize sem with invalid values(-1,-1)
 * 
 * sem_id = valid
 * initial_value = -1
 * max_value = -1
 * context = task
 */
void gzis_sem_test_011()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, -1, -1);
    STF_CHECK_RETURN_TEST(ret);
}

/**
 * @brief Initialize sem with invalid values(0,-1)
 * 
 * sem_id = valid
 * initial_value = 0
 * max_value = -1
 * context = task
 */
void gzis_sem_test_012()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, 0, -1);
    STF_CHECK_RETURN_TEST(ret);
}


static int sem_test_013_valid = 1;

static void sem_test_013_timer_handler(uval_gt param)
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, 0, 1);
    STF_CHECK_RETURN_TEST(ret);
    sem_test_013_valid = 0;
}

/**
 * @brief Initialize sem in isr with valid params
 * 
 * sem_id = valid
 * initial_value = 0
 * max_value = 1
 * context = interrupt
 * pre_condition = gzis_timer_create, gzis_timer_set_time
 */
void gzis_sem_test_013()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    timer_gt timer_id;
    itimerspec_gt timerspec;

    ret = gzis_timer_create(system_clock, &timer_id, sem_test_013_timer_handler, (uval_gt) NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    timerspec.it_value.second=1;
    timerspec.it_value.nanosecond=0;
    timerspec.it_interval.second=0;
    timerspec.it_interval.nanosecond=0;

    ret = gzis_timer_set_time(timer_id, &timerspec, NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    while(sem_test_013_valid);
}

static int sem_test_014_valid = 1;

static void sem_test_014_timer_handler(uval_gt param)
{
    gzis_status_gt ret;
    ret = gzis_sem_init(NULL, 0, 1);
    STF_CHECK_RETURN_TEST(ret);
    sem_test_014_valid = 0;
}

/**
 * @brief Initialize sem in isr with invalid params(sem_id=NULL)
 * 
 * sem_id = invalid (NULL)
 * initial_value = 0
 * max_value = 1
 * context = interrupt
 * pre_condition = gzis_timer_create, gzis_timer_set_time
 */
void gzis_sem_test_014()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    timer_gt timer_id;
    itimerspec_gt timerspec;

    ret = gzis_timer_create(system_clock, &timer_id, sem_test_013_timer_handler, (uval_gt) NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    timerspec.it_value.second=1;
    timerspec.it_value.nanosecond=0;
    timerspec.it_interval.second=0;
    timerspec.it_interval.nanosecond=0;

    ret = gzis_timer_set_time(timer_id, &timerspec, NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    while(sem_test_014_valid);
}

static int sem_test_015_valid = 1;

static void sem_test_015_timer_handler(uval_gt param)
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, -1, 0);
    STF_CHECK_RETURN_TEST(ret);
    sem_test_015_valid = 0;
}

/**
 * @brief Initialize sem in isr with invalid params(-1,0)
 * 
 * sem_id = valid
 * initial_value = -1
 * max_value = 0
 * context = interrupt
 * pre_condition = gzis_timer_create, gzis_timer_set_time
 */
void gzis_sem_test_015()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    timer_gt timer_id;
    itimerspec_gt timerspec;

    ret = gzis_timer_create(system_clock, &timer_id, sem_test_013_timer_handler, (uval_gt) NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    timerspec.it_value.second=1;
    timerspec.it_value.nanosecond=0;
    timerspec.it_interval.second=0;
    timerspec.it_interval.nanosecond=0;

    ret = gzis_timer_set_time(timer_id, &timerspec, NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    while(sem_test_015_valid);
}

static int sem_test_016_valid = 1;

static void sem_test_016_timer_handler(uval_gt param)
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, 0, -1);
    STF_CHECK_RETURN_TEST(ret);
    sem_test_016_valid = 0;
}

/**
 * @brief Initialize sem in isr with invalid params(0,-1)
 * 
 * sem_id = valid
 * initial_value = 0
 * max_value = -1
 * context = interrupt
 * pre_condition = gzis_timer_create, gzis_timer_set_time
 */
void gzis_sem_test_016()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    timer_gt timer_id;
    itimerspec_gt timerspec;

    ret = gzis_timer_create(system_clock, &timer_id, sem_test_013_timer_handler, (uval_gt) NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    timerspec.it_value.second=1;
    timerspec.it_value.nanosecond=0;
    timerspec.it_interval.second=0;
    timerspec.it_interval.nanosecond=0;

    ret = gzis_timer_set_time(timer_id, &timerspec, NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    while(sem_test_016_valid);
}

static int sem_test_017_valid = 1;

static void sem_test_017_timer_handler(uval_gt param)
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, -1, -1);
    STF_CHECK_RETURN_TEST(ret);
    sem_test_017_valid = 0;
}

/**
 * @brief Initialize sem in isr with invalid params(-1,-1)
 * 
 * sem_id = valid
 * initial_value = -1
 * max_value = -1
 * context = interrupt
 * pre_condition = gzis_timer_create, gzis_timer_set_time
 */
void gzis_sem_test_017()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    timer_gt timer_id;
    itimerspec_gt timerspec;

    ret = gzis_timer_create(system_clock, &timer_id, sem_test_013_timer_handler, (uval_gt) NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    timerspec.it_value.second=1;
    timerspec.it_value.nanosecond=0;
    timerspec.it_interval.second=0;
    timerspec.it_interval.nanosecond=0;

    ret = gzis_timer_set_time(timer_id, &timerspec, NULL);
    STF_CHECK_RETURN(ret, ERR_NO_ERROR);

    while(sem_test_017_valid);
}

/**
 * @brief Initialize sem with valid values(0,0)
 * 
 * sem_id = valid
 * initial_value = 0
 * max_value = 0
 * context = task
 */
void gzis_sem_test_005()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, 0, 0);
    STF_CHECK_RETURN_TEST(ret);
}

/**
 * @brief Initialize binary sem with valid values(0,1)
 * 
 * sem_id = valid
 * initial_value = 0
 * max_value = 1
 * context = task
 */
void gzis_sem_test_006()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, 0, 1);
    STF_CHECK_RETURN_TEST(ret);
}

/**
 * @brief Initialize binary sem with valid values(1,1)
 * 
 * sem_id = valid
 * initial_value = 1
 * max_value = 1
 * context = task
 */
void gzis_sem_test_007()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, 1, 1);
    STF_CHECK_RETURN_TEST(ret);
}

/**
 * @brief Initialize binary sem with valid values(0,GZIS_SEM_VALUE_MAX)
 * 
 * sem_id = valid
 * initial_value = 0
 * max_value = GZIS_SEM_VALUE_MAX
 * context = task
 */
void gzis_sem_test_008()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, 0, GZIS_SEM_VALUE_MAX);
    STF_CHECK_RETURN_TEST(ret);
}

/**
 * @brief Initialize binary sem with valid values(GZIS_SEM_VALUE_MAX,GZIS_SEM_VALUE_MAX)
 * 
 * sem_id = valid
 * initial_value = GZIS_SEM_VALUE_MAX
 * max_value = GZIS_SEM_VALUE_MAX
 * context = task
 */
void gzis_sem_test_009()
{
    gzis_status_gt ret;
    sem_gt sem_id;
    ret = gzis_sem_init(&sem_id, GZIS_SEM_VALUE_MAX, GZIS_SEM_VALUE_MAX);
    STF_CHECK_RETURN_TEST(ret);
}