/*-------------------------------------------------------------------------
 *
 * disablelogerror.c
 *      disable logging for some sqlerrcode messages
 *
 * Copyright (c) 2022, Francisco Miguel Biete Banon
 *
 * This code is released under the PostgreSQL licence, as given at
 *  http://www.postgresql.org/about/licence/
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "miscadmin.h"
#include "utils/guc.h"
#include "utils/varlena.h"

#include "disablelogerror.h"

/* Old hook storage for loading/unloading of the extension */
static emit_log_hook_type prev_emit_log_hook = NULL;

/* Hook functions */
static void pgdisablelogerror_emit_log(ErrorData *edata);

/* GUC variable for sqlerrcode, which defines the codes to hide. */
char *pgdisablelogerror_sqlerrcode = NULL;

static bool guc_check_sqlerrcode(char **newVal, void **extra, GucSource source);

/*
 * Extension Init Callback
 */
void _PG_init(void) {
  if (!process_shared_preload_libraries_in_progress)
    ereport(ERROR, (errcode(ERRCODE_OBJECT_NOT_IN_PREREQUISITE_STATE),
          errmsg("pgdisablelogerror must be loaded via shared_preload_libraries")));

  DefineCustomStringVariable(
    "pgdisablelogerror.sqlerrcode",
    "Specifies which sql error codes will not cause logging in the server. "
    "Multiple codes can be provided using a comma-separated list.",
    NULL,
    &pgdisablelogerror_sqlerrcode,
    "",
    PGC_SIGHUP,
    GUC_LIST_INPUT | GUC_NOT_IN_SAMPLE,
    guc_check_sqlerrcode,
    NULL,
    NULL);

  ereport(LOG, (errmsg("pgdisablelogerror extension initialized")));

  EmitWarningsOnPlaceholders("pgdisablelogerror");

  prev_emit_log_hook = emit_log_hook;
  emit_log_hook = pgdisablelogerror_emit_log;
}

/*
 * Extension Fin Callback
 */
void _PG_fini(void) {
  emit_log_hook = prev_emit_log_hook;

  ereport(LOG, (errmsg("pgdisablelogerror extension removed")));
}

/*
 * Hook to emit_log - write the record to the audit or send it to the default
 * logger
 */
static void pgdisablelogerror_emit_log(ErrorData *edata) {
  char *rawVal;
  List *sqlerrcodeList;
  ListCell *lt;

  /* Work with a duplicate of the list, SplitGUCList modifies the objet */
  rawVal = pstrdup(pgdisablelogerror_sqlerrcode);

  /* Split the list by comma */
  if (!SplitGUCList(rawVal, ',', &sqlerrcodeList)) {
    pfree(rawVal);
    list_free(sqlerrcodeList);

    if (prev_emit_log_hook)
      prev_emit_log_hook(edata);
  }

  // Check all the items
  foreach(lt, sqlerrcodeList) {
    char *token = (char *) lfirst(lt);

    // printf("Checking %s %s\n", token, unpack_sql_state(edata->sqlerrcode));

    // If sqlerrcode is in the disable list, mark is as not logging
    if (strcmp(unpack_sql_state(edata->sqlerrcode), token) == 0) {
      edata->output_to_server = false;
      break;
    }
  }

  pfree(rawVal);
  list_free(sqlerrcodeList);

  // Log
  if (prev_emit_log_hook)
    prev_emit_log_hook(edata);
}

static bool
guc_check_sqlerrcode(char **newVal, void **extra, GucSource source)
{
    List *flagRawList;
    char *rawVal;

    /* Make sure newval is a comma-separated list of tokens. */
    rawVal = pstrdup(*newVal);
    if (!SplitGUCList(rawVal, ',', &flagRawList))
    {
        GUC_check_errdetail("List syntax is invalid");
        list_free(flagRawList);
        pfree(rawVal);
        return false;
    }

    pfree(rawVal);
    list_free(flagRawList);

    return true;
}
