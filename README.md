# pgdisablelogerror

[pgdisablelogerror](https://github.com/fmbiete/pgdisablelogerror) is a PostgreSQL extension that will disable server logging for specified sql error codes.

This will allow you to omit messages for acceptable errors (for example unique key violations - 23505).


## Build
```
make install USE_PGXS=1
```

## Installation
- Build the extension
- Add pgdisablelogerror to "shared_preload_libraries" in postgresql.conf
- Restart PostgreSQL to reload new shared library
- Create extension in postgres database (you only need to create it under postgres database)

```
postgres=# CREATE EXTENSION pgdisablelogerror;
```

## Configuration

### pgdisablelogerror.sqlerrcode
Comma separated list of sql error codes to skip logging.

**Scope**: System

**Default**: ''

Empty or NULL will disable the extension.

#### Identify sqlerrcode
Add the following code to _log_line_prefix_: _%e_
