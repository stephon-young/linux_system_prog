// for getInt and getLong
//

#if !defined(PARSER_H__)
#define PARSER_H__

#define PSR_NONNEG 01
#define PSR_GT_0 02

#define PSR_ANY_BASE 0100
#define PSR_BASE_8 0200
#define PSR_BASE_16 0400

long getLong(const char *arg, int flags, const char *name);
int getInt(const char *arg, int flags, const char *name);

#endif // PARSER_H__