//Editor-Info: -*- C++ -*-
//


#ifndef EDITTASK_H
#define EDITTASK_H

#include <doors/core.h>
#include <doors/inputask.h>

#include <doors/otl_dcl.h>

#include <termios.h>

//
//Class: EditTask
//
//Inherited: InputTask
//
//Description: 
//      EditTask is derived from InputTask. It contains a buffer to output
//      characters and various line-editing functions.
//      EditTask puts terminal into raw mode, so there may be problems
//      with applications that try to read directly from terminal.
//      Functions e_setterm() and e_resetterm() can be used to set 
//      terminal into wanted mode.
//      Default bindings are:
//        C-h	backspace,
//        DEL	backspace,
//        C-d	end-of-file,
//        C-m	end-of-line,
//        C-j	end-of-line,
//        E-[A	previous-history,
//        E-[B	next-history,
//        E-[C	forward-char,
//        E-[D	backward-char,
//        E-OA	previous-history,
//        E-OB	next-history,
//        E-OC	forward-char,
//        E-OD	backward-char,
//        C-p	previous-history,
//        C-n	next-history,
//        C-f	forward-char,
//        C-b	backward-char,
//        C-e	goto-end,
//        C-a	goto-begin,
//        C-l	redraw-line,
//        C-k	kill-to-end,
//        C-u	kill-to-begin,
//        C-y	yank
//
class EditTask : public InputTask {
    class OutputBuffer {
	friend class EditTask;
      public:
        OutputBuffer (void);	
        void outChar (Byte);
	void backChar (Byte);
	void eraseChar (Byte);
	void outString (const Byte *);
	void flush (void);
	void beep (void);
	void eol (void);
	
	Uint16 cursor;
	
      private:
	Frame outputBuffer;
	int fd;
    };
    
    enum RegionCommand {
	DRAW,
	EMPTY,
	COUNT
	};

  public:
    EditTask (Scheduler *s, IoHandler *io);
    ~EditTask (void);

    bool execute (EventType, Message *);
    bool connect (Port *);
    void setTerm (void) { e_setterm (device->getFd()); }
    void resetTerm (void) { e_resetterm (device->getFd()); }

    static int k_nop (EditTask *, Frame *);
    static int k_self_insert (EditTask *, Frame *);
    static int k_backspace (EditTask *, Frame *);
    static int k_delete (EditTask *, Frame *);
    static int k_eof (EditTask *, Frame *);
    static int k_eol (EditTask *, Frame *);
    static int k_prev_history (EditTask *, Frame *);
    static int k_next_history (EditTask *, Frame *);
    static int k_forward (EditTask *, Frame *);
    static int k_backward (EditTask *, Frame *);
    static int k_end (EditTask *, Frame *);
    static int k_begin (EditTask *, Frame *);
    static int k_redraw (EditTask *, Frame *);
    static int k_kill_end (EditTask *, Frame *);
    static int k_kill_begin (EditTask *, Frame *);
    static int k_yank (EditTask *, Frame *);

    void gotoColumn (FrameSize);
    int drawRegion (int start, int end, enum RegionCommand);
    void clearLine (void);


    void e_init ();
    void e_bind_key ( const char *, const char * );
    void e_unbind_key ( const char * );
    void e_bindkeys (void);
    void e_unbindkeys (void);

    void history (bool b);

    void pushHistory (Frame *);
    Frame *getHistory (int);

    void set_ckey ( unsigned char, const char * );
    void set_key ( const char *, unsigned int );
    int match_key ( const char *, int );
    int match_key ( const Frame *);


#ifdef COMPLETE
    char  **(*complete_lister) ();
    char  **make_list();
    int     k_complete(), k_complete_list();
#endif

  private:
    void e_setterm ( int );
    void e_resetterm ( int );

    HistoryList historyList;
    bool doHistory;
    int curhist, histsize;
    int xpos;
    Frame rivi, yank;
    OutputBuffer outbuf;

    struct termios old_termio;
};



extern int charlen ( unsigned char );

// extern char *strdup ( char * );
// extern char *strndup ( char *, int );

typedef int (* KeyFunction) (EditTask *, Frame *);

#define MAXKEYS 100
#define MAXHIST 50

struct k_com {
    int     com;
    char   *str;
};

extern struct k_com comlist[MAXKEYS];

struct k_func 
{
    const char   *name;
    KeyFunction ptr;
};

extern struct k_func funlist[];

#endif
