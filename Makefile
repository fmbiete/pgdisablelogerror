# pgdisablelogerror/Makefile

MODULE_big = pgdisablelogerror
OBJS = pgdisablelogerror.o disablelogerror.o

EXTENSION = pgdisablelogerror
DATA = pgdisablelogerror--1.0.sql
PGFILEDESC = "pgdisablelogerror - Don't log some error messages in the server log"

PG_LDFLAGS = -lz

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
