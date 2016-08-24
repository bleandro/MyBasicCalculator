
extern int lookahead;

extern int gettoken (FILE *);

extern FILE *source;

extern void match (int);

/* syntax names */
void expr (void);
void rest (void);
void term (void);
void quoc (void);
void fact (void);
int addop (void);
int mulop (void);
