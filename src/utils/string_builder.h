//
// Created by Vinetos on 10/04/2021.
//

#ifndef HEYMORTY_SRC_STRING_BUILDER_H_
#define HEYMORTY_SRC_STRING_BUILDER_H_

#define SB_FAILURE                0
#define SB_MAX_FRAG_LENGTH        4096

typedef struct _StringFragment {
  struct _StringFragment *next;
  unsigned long length;
  char *str;
} StringFragment;

typedef struct _StringBuilder {
  struct _StringFragment *root;
  struct _StringFragment *trunk;
  unsigned long length;
} StringBuilder;

StringBuilder *sb_create();
unsigned long sb_empty(StringBuilder *sb);
unsigned long sb_append(StringBuilder *sb, const char *str);
unsigned long sb_appendf(StringBuilder *sb, const char *format, ...);
unsigned long sb_appendf_l(StringBuilder *sb, const char *format,
                           unsigned long length, ...);
char * sb_format(const char *format, ...);
char *sb_concat(StringBuilder *sb);
void sb_reset(StringBuilder *sb);
void sb_free(StringBuilder *sb);

#endif //HEYMORTY_SRC_STRING_BUILDER_H_
