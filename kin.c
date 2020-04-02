#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

enum {
  TITLE = 1 << 0,
  META = 1 << 1,
  TEXT = 1 << 2,
};

static bool consume(char** p, char* expect) {
  char* q = *p;
  while (*expect) {
    if (*q++ != *expect++) { return false; }
  }
  *p = q;
  return true;
}

static char* consume_until(char* p, char* chars) {
  while (*p && !strchr(chars, *p)) {
    p++;
  }
  return p;
}

static char* copy_until(char* dst, char* src, char* chars) {
  while (*src && !strchr(chars, *src)) {
    *dst++ = *src++;
  }
  *dst = '\0';
  return src;
}

static int write_metadata_el(FILE* out, int state);

static int write_title_el(FILE* out, int state) {
  if (state & TITLE) {
    fprintf(out, "</h5>");
    return write_metadata_el(out, state);
  }
  
  fprintf(out, "<li><h5>");
  return TITLE;
}

static int write_text_el(FILE* out, int state) {
  if (state & TEXT) {
    fprintf(out, "</p></li>");
    return write_title_el(out, state);
  }

  fprintf(out, "<p>");
  return TEXT;
}

static int write_metadata_el(FILE* out, int state) {
  if (state & META) {
    fprintf(out, "</h5>");
    return write_text_el(out, state);
  }

  fprintf(out, "<h5>");
  return META;
}

static int process_title(FILE* out, char* line, int state) {
  char title[256], name[128];
  line = copy_until(title, line, "(");
  fprintf(out, "%s", title);
  
  if (consume(&line, "(")) {
    line = copy_until(name, line, ")");
    fprintf(out, " (%s)", name);
  }

  return write_title_el(out, state);
}

static int process_meta(FILE* out, char* line, int state) {
  char page[32], date[32], year[8];

  bool note = consume(&line, "Your Note");
  
  line = consume_until(line, "0123456789");
  line = copy_until(page, line, "|");
  
  if (consume(&line, "| Loc")) {
    line = consume_until(line, "|");
  }
  
  if (consume(&line, "| ")) {
    line = consume_until(line, ",");
    // skip ", "
    line += 2;
    line = copy_until(date, line, ",");
    
    // skip ", "
    line += 2;
    line = copy_until(year, line, " ");
  }

  fprintf(out, "%s, %s on page %s", date, year, page);
  if (note) {
    fprintf(out, " ― Your Note");
  }
  
  return write_metadata_el(out, state);
}

static int process_line(FILE* out, char* line, int state) {
  if (state & TITLE) {
    return process_title(out, line, state);
  }
  
  if (state & META && consume(&line, "- ")) {
    return process_meta(out, line, state);
  }

  if (state & TEXT && consume(&line, "==========")) {
    // end of text
    return write_text_el(out, state);
  }

  // text line
  fprintf(out, "%s", line);
  return TEXT;
}

int main(int argc, char** argv) {
  FILE* in = stdin;

  for (int i = 1; i < argc; i++) {
    in = fopen(argv[i], "rb");
    if (!in) {
      fprintf(stderr, "error: failed to open input file\n");
      exit(1);
    }
  }

  fprintf(stdout, "<html><head><title>Kindle Highlights</title><style>");
  fprintf(stdout,
    "body{margin:60 auto;max-width:750px;line-height:1.6;"
    "font-family:sans-serif;padding:0 10px}"
    "ul{list-style:none;display:flex;flex-direction:column-reverse}"
    "li{flex:0 0 auto}"
    "h5{margin:0}"
    "p{margin-bottom:32px}"
  );
  fprintf(stdout, "</style></head><body><ul>");

  char line[4096];
  // file begins with the title of first book
  int state = write_title_el(stdout, 0);
  while (fgets(line, sizeof(line), in)) {
    state = process_line(stdout, line, state);
  }
  
  fprintf(stdout, "</ul></body></html>");
  return 0;
}
