
#define CNTLQ       0x11
#define CNTLS       0x13
#define DEL         0x7F
#define BACKSPACE   0x08
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B
 void ltoa(long int n,char *str);
int sendchar (int ch);
int getkey (void);
int sendchar2 (int ch);
int getkey2 (void);
void jtag_Disable(void);
void send_static_string(const char *str);
void send_static_string2(const char *str);
void send_string(const char *str);								  
void send_string2(char *str);
char get_udir(char *value);
//void watchdog_init(void);
//void watchdog_reset(void);
void clear_array(char*array,int size);
