/*-------------------------------------------------------------------------
 *
 * pgdisablelogerror.c
 *      disable loggin for some sqlerrcode messages
 *
 * Copyright (c) 2022, Francisco Miguel Biete Banon
 *
 * This code is released under the PostgreSQL licence, as given at
 *  http://www.postgresql.org/about/licence/
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "utils/guc.h"

PG_MODULE_MAGIC;

#include "disablelogerror.h"
