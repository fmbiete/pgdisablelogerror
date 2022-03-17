/*-------------------------------------------------------------------------
 *
 * disablelogerror.h
 *      disable logging for some sqlerrcode messages
 *
 * Copyright (c) 2022, Francisco Miguel Biete Banon
 *
 * This code is released under the PostgreSQL licence, as given at
 *  http://www.postgresql.org/about/licence/
 *-------------------------------------------------------------------------
 */
#ifndef PGDISABLELOGERROR_H
#define PGDISABLELOGERROR_H

/* initialization functions */
void _PG_fini(void);
void _PG_init(void);

#endif
