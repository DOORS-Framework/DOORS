//Editor-Info: -*- C++ -*-
//

#include <doors/editask.h>
#include <doors/vstream.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//#define COMPLETE  
// Completion does not work yet.

#ifdef COMPLETE
#include <sys/types.h>
#include <dirent.h>
#endif

struct k_com comlist[MAXKEYS];


//
//Constructor: OutputBuffer
//
//Parameters: 
//
EditTask :: OutputBuffer :: OutputBuffer (void): cursor (0)
{
} 


//
//Constructor: EditTask
//
//Parameters: 
//       Scheduler *, IoHandler *
//
//Description: 
//      The constructor initializes variables and calls base class 
//      constructors.
//      Terminal is set to raw mode, and keys are bound to default values
//
EditTask :: EditTask (Scheduler *s, IoHandler *io) 
: InputTask (s, io, "/dev/tty", Device::READ | Device::WRITE), 
  doHistory (true), curhist (0), histsize (0), xpos (0)

{
    e_bindkeys ();
    histsize = 0;
    return;
}


//
//Destructor: EditTask
//
//Description: 
//      Terminal is returned to original mode.
//
EditTask :: ~EditTask (void)
{
    e_resetterm (device->getFd ());
    e_unbindkeys ();
}


//
//Function: connect
//
//Member-Of: InputTask
//
//Parameters:
//      Port *
//
//Return:
//      bool
//
//Description:
//      In connection of task, the file is opened and set up for reading.
//      Zero return value means that the open was succesful. 
//  
bool EditTask :: connect (Port *at)
{
    if (!InputTask :: connect (at)) {
	return false;
    }
    e_setterm (device->getFd ());	
    outbuf.fd = device->getFd ();
    return true;
}


//
//Function: run
//
//Member-Of: EditTask
//
//Description: 
//      run-function is called when the FileDevice has been able 
//      to read something. Received chars are searched from command-key
//      list. If a command is found, corresponding function is called.
//
bool EditTask :: execute (EventType, Message *msg)
{

  if (msg->getTarget () == &devIf) {
  
    switch (msg->getType()) {
      
    case (DevIf :: open) : {
      delete msg;
      break;
    }
    
    case (DevIf :: close) : {
      device->close();
      getIoHandler()->forget(device);
      delete msg;
      break;
    }
    case (DevIf :: read) : {
      buffer = ((DevIf::Read *) msg)->data;
      device->readBytes (StreamDevice :: immediate);
      while (buffer.length() > 0) {
	// we got something from luser
	
	// if there's no "current" initialize system.
	if (!current) {
	  e_init ();
	  current = new InputMessage;	    
	}
	
	int exitCode = 0, match, matchlen;
	
	match = match_key (&buffer);
	
	if (match == -1)
	  break;
	// several matches found - wait for more chars
	
	
	// if key not found print it char by char (match = 0)
	matchlen = match ? strlen(comlist[match].str) : 1;
	
	
	Frame &tmp = buffer.split (matchlen);
	exitCode = (funlist[comlist[match].com].ptr) (this, &tmp);
	delete &tmp;
	
	if (exitCode) {
	  if (doHistory) {
	    if (rivi.length () > 0) {
	      pushHistory (&rivi);
	    }
	  }
	  current->data = rivi;
	  current->data.putLast ('\n');
	  Message *msg =  current; // to fix AT&T problems
	  output.putMessage(msg);
	  current = 0;
	}	
      }
      delete msg;
      break;
    }
    }
  }

  return true;
}
//
// edit functions
//

//
//Function: e_init
//
//Member-Of: EditTask
//
//Description: 
//      Function initializes commandline editing specific variables.
//      Function is called at beginning of each line.
//
void EditTask :: e_init () {
    curhist = -1;
    xpos = 0;

    rivi.destroy ();
    return;
}


//
//Function: e_setterm
//
//Member-Of: EditTask
//
//Parameters: 
//       int fd
//
//Description: 
//       setterm sets terminal to raw mode
//       Normally chars are read from terminal after user has typed whole line
//
void EditTask :: e_setterm(int fid)
{
    struct termios newt;
    tcgetattr(fid, &newt);
    old_termio = newt;

    newt.c_iflag &= ~(INLCR | ICRNL);
#ifdef MAIN
    newt.c_lflag &= ~(ISIG | ICANON | ECHO);
#else
    newt.c_lflag &= ~(ICANON | ECHO);
#endif
    newt.c_cc[VTIME] = 0;
    newt.c_cc[VMIN] = 1;

    tcsetattr(fid, TCSANOW, &newt);
    return;
}


//
//Function: e_bindkeys
//
//Member-Of: EditTask
//
//Description: 
//       Function binds default values for command keys
//
void EditTask :: e_bindkeys (void)
{
    int     i;
    for (i = 0; i < MAXKEYS; i++)
	comlist[i].str = NULL;
    // must be first in comlist
    set_ckey(0xff, "self-insert-key");

    // take some keys from termios structure
    // set_ckey (old_termio.c_cc[VEOL], F_EOL);
    // set_ckey (old_termio.c_cc[VEOF], F_EOF);
    // set_ckey(old_termio.c_cc[VERASE], "backspace");
    // set_ckey(old_termio.c_cc[VKILL], "kill-to-begin");

    e_bind_key("\010", "backspace");
    e_bind_key("\0177", "backspace");
    e_bind_key("\04", "end-of-file");
    e_bind_key("\r", "end-of-line");
    e_bind_key("\n", "end-of-line");
    e_bind_key("\033[A", "previous-history");
    e_bind_key("\033[B", "next-history");
    e_bind_key("\033[C", "forward-char");
    e_bind_key("\033[D", "backward-char");
    e_bind_key("\033OA", "previous-history");
    e_bind_key("\033OB", "next-history");
    e_bind_key("\033OC", "forward-char");
    e_bind_key("\033OD", "backward-char");
    e_bind_key("\020", "previous-history");
    e_bind_key("\016", "next-history");
    e_bind_key("\06", "forward-char");
    e_bind_key("\02", "backward-char");
    e_bind_key("\05", "goto-end");
    e_bind_key("\01", "goto-begin");
    e_bind_key("\014", "redraw-line");
    e_bind_key("\013", "kill-to-end");
    e_bind_key("\025", "kill-to-begin");
    e_bind_key("\031", "yank");
    e_bind_key("\024", "yank");

#ifdef COMPLETE
    e_bind_key("\011", "complete-file");
    /* set default completer */
    complete_lister = make_list;
#endif
}


//
//Function: e_resetterm
//
//Member-Of: EditTask
//
//Parameters: 
//       int fd
//
//Description: 
//       resetterm sets terminal back to original mode
//
void    EditTask :: e_resetterm(int fid)
{
    old_termio.c_iflag |= INLCR | ICRNL;
    old_termio.c_lflag |= ICANON | ECHO;
    tcsetattr(fid, TCSANOW, &old_termio);
}


//
//Function: e_bind_key
//
//Member-Of: EditTask
//
//Parameters: 
//       const char *string, const char *function
//
//Description: 
//       Binds 'string' to 'function'
//
void    EditTask :: e_bind_key(const char *str, const char *funn)
{
    int     i;
    for (i = 0; funlist[i].name; i++)
	if (!strcmp(funlist[i].name, funn))
	    break;
    if (!funlist[i].name) {
	vout << "illegal function " << funn << endv;
	return;
    }
    set_key(str, i);
}


//
//Function: e_unbind_key
//
//Member-Of: EditTask
//
//Parameters: 
//       const char *string
//
//Description: 
//       Removes bounding of 'string''
//
void    EditTask :: e_unbind_key(const char *str)
{
    int     i;
    switch (i = match_key(str, strlen(str))) {
      case -1:
      case 0:
	    vout << "key not bound!" << endv;
	return;
    }
    delete (comlist[i].str);
    comlist[i].com = 0;
    comlist[i].str = 0;
}


//
//Function: e_unbindkeys
//
//Member-Of: EditTask
//
//
//Description: 
//       Removes bounding of keys
//
void EditTask :: e_unbindkeys (void)
{
    for (int i = 0; i < MAXKEYS; i++)
    {
        if (comlist[i].str)
	{
           delete comlist[i].str;
           comlist[i].com = 0;
           comlist[i].str = 0;
        }
    }

}


//
//Function: match_key
//
//Member-Of: EditTask
//
//Parameters: 
//      const Frame *
//
//Return: 
//      int
//
//Description: 
//      Function tries to match Frame to list of command-keys.
//      Upon successful index of command is returned.
//      If match is ambiguous (e.g. more than one command beginning with
//      matched string) -1 is returned. If Frame does not match at all,
//      function return zero.
//
int EditTask :: match_key(const Frame *frm)
{
    FrameIterator iterator (frm);
    int len = (int) frm->length ();
    int match = 0, i, j;
    const char *p;
    for (i = 0; i < MAXKEYS; i++) {
	if (comlist[i].str) {
	    for (p = comlist[i].str, j = 0; *p && j < len; p++, j++) {
		if (iterator.read (j) != (Byte)*p) {
		    break;
		}
	    }
	    if (!*p || j == len) {
		if (match)
		    return -1;
		else
		    match = i;
	    }
	}
    }
    return (match);
}


//
//Function: match_key
//
//Member-Of: EditTask
//
//Parameters: 
//      const char *string, int length
//
//Return: 
//      int
//
//Description: 
//      This function works in a similar 
//      way as EditTask :: match_key(const Frame *frm)
//      And exists only for historical reasons.
//
int    EditTask ::  match_key(const char *str, int len)
{
    int     match = 0, i;
    for (i = 0; i < MAXKEYS; i++)
	if (comlist[i].str && !strncmp(comlist[i].str, str, len))
	    match = match ? -1 : i;
    return (match);
}


bool decrement (const Frame *, const void *counter)
{    
    return (--*((int *)counter) > 0) ? true : false;
}

void EditTask :: pushHistory(Frame *line)
{
    int counter = MAXHIST;
    while (histsize > MAXHIST) {
	historyList.remove (&counter, decrement);
	histsize--;
    }
    historyList.insert (&line->copy());
    histsize++;
}

Frame *EditTask :: getHistory(int index)
{
    Frame *hst = 0;
    if (index < histsize && index >= 0) {
	HistoryListIterator iterator (&historyList);
	for (int i = 0; i <= index; i++)
	    hst = iterator.next();
    }
    return hst;
}


void EditTask :: history (bool b) {

    doHistory = b;
}


//
// static key functions
//

//
//Function: k_function
//Member-Of: EditTask
//Description:
//  Key functions are bound to keys. They perform the actual line-editing
//
//  The k_function is one of 
//      EditTask::k_nop,
//      EditTask::k_self_insert,
//      EditTask::k_backspace,
//      EditTask::k_delete,
//      EditTask::k_eof,
//      EditTask::k_eol,
//      EditTask::k_prev_history,
//      EditTask::k_next_history,
//      EditTask::k_forward,
//      EditTask::k_backward,
//      EditTask::k_end,
//      EditTask::k_begin,
//      EditTask::k_redraw,
//      EditTask::k_kill_end,
//      EditTask::k_kill_begin,
//      EditTask::k_yank .
//
//Parameters: 
//       EditTask *self, Frame *buffer
//
//Return: 
//      int

int EditTask :: k_self_insert(EditTask *self, Frame *buf)
{
    /* make room for string */
    int len = (int) buf->length ();
    Frame &tmp = self->rivi.split (self->xpos);
    tmp.combine (*buf);
    tmp.combine (self->rivi);
    self->rivi = tmp;
    delete &tmp;

    /* draw rest of line */
    self->drawRegion (self->xpos, -1, DRAW);
    self->gotoColumn (self->xpos += len);
    return (0);
}

int EditTask :: k_nop (EditTask *, Frame *)
{
    return (0);
}

int EditTask :: k_backspace(EditTask *self, Frame *)
{
    if (self->xpos > 0) {

	Frame &tmp = self->rivi.split (--self->xpos);
	Byte b = self->rivi.getFirst ();
	tmp.combine (self->rivi);
	self->rivi = tmp;
        delete &tmp;

	self->outbuf.backChar(b);
	self->drawRegion (self->xpos, -1, DRAW);
	self->outbuf.eraseChar (b);
	self->gotoColumn (self->xpos);
    } else
	self->outbuf.beep();
    return (0);
}

int EditTask :: k_delete(EditTask *self, Frame *)
{
    if (self->xpos < self->rivi.length ()) {

	Frame &tmp = self->rivi.split (self->xpos);
	Byte b = self->rivi.getFirst ();
	tmp.combine (self->rivi);
	self->rivi = tmp;
        delete &tmp;

	self->drawRegion (self->xpos, -1, DRAW);
	self->outbuf.eraseChar (b);
	self->gotoColumn (self->xpos);
    } else {
#ifdef COMPLETE
	k_complete_list();
#else
	self->outbuf.beep();
#endif
    }
    return (0);
}

int EditTask :: k_eof(EditTask *self, Frame *frm)
{
    if (self->rivi.length () == 0) {
	self->outbuf.outString ((const Byte *)"[EOF]");
	self->outbuf.eol ();
	return (2);
    }
    k_delete(self, frm);
    return (0);
}

int EditTask :: k_eol(EditTask *self, Frame *)
{
    self->outbuf.eol ();
    return (1);
}

void EditTask :: clearLine (void)
{
    gotoColumn (0);
    drawRegion (0, -1, EMPTY);
    gotoColumn (xpos = 0);
}

int EditTask :: k_prev_history(EditTask *self, Frame *)
{
    Frame *hst = 0;

    if (self->doHistory) {
	if (self->curhist < self->histsize - 1 && 
	    (hst = self->getHistory (self->curhist + 1))) {
	    self->curhist++;
	    self->clearLine ();
	    self->rivi = hst->copy();
	    self->drawRegion (0, -1, DRAW);
	    self->xpos = (int) self->rivi.length ();
	    self->outbuf.flush();
	    return (0);
	}
    }
    self->outbuf.beep();
    return (0);
}

int EditTask :: k_next_history(EditTask *self, Frame *)
{
    Frame *hst = 0;

    if (self->doHistory) {
	if (self->curhist > 0 &&
	    (hst = self->getHistory (self->curhist - 1))) {
	    self->curhist--;
	    self->clearLine ();
	    self->rivi = hst->copy();
	    self->drawRegion (0, -1, DRAW);
	    self->xpos = (int) self->rivi.length ();
	    self->outbuf.flush();
	    return (0);
	} else if (self->curhist == 0) {
	    self->curhist--;
	    self->clearLine ();
	    self->rivi.destroy ();
	    self->xpos = 0;
	    return (0);
	}
    }
    self->outbuf.beep();
    return (0);
}

int EditTask :: k_forward(EditTask *self, Frame *)
{
    if (self->xpos < self->rivi.length ())
	self->gotoColumn (++self->xpos);
    return (0);
}

int EditTask :: k_backward(EditTask *self, Frame *)
{
    if (self->xpos > 0)
	self->gotoColumn (--self->xpos);
    return (0);
}

int EditTask :: k_end(EditTask *self, Frame *)
{
    self->gotoColumn (self->xpos = (int) self->rivi.length ());
    return (0);
}

int EditTask :: k_begin(EditTask *self, Frame *)
{
    self->gotoColumn (self->xpos = 0);
    return (0);
}

int EditTask :: k_redraw(EditTask *self, Frame *)
{
    self->outbuf.eol ();
    self->drawRegion(0, -1, DRAW);
    self->gotoColumn (self->xpos);
    return (0);
}

int EditTask :: k_kill_end(EditTask *self, Frame *)
{
    self->drawRegion (self->xpos, -1, EMPTY);
    self->gotoColumn (self->xpos);
    self->yank = self->rivi;
    Frame &tmp = self->yank.split (self->xpos);
    self->rivi = tmp;
    delete &tmp;
    return (0);
}

int EditTask :: k_kill_begin(EditTask *self, Frame *)
{
    self->gotoColumn (0);
    self->drawRegion (self->xpos, -1, DRAW);
    self->drawRegion (0, self->xpos, EMPTY);
    self->gotoColumn (0);

    Frame &tmp = self->rivi.split (self->xpos);
    self->yank = tmp;
    delete &tmp;

    self->xpos = 0;
    return (0);
}

int EditTask :: k_yank(EditTask *self, Frame *)
{
    Frame &tmp = self->rivi.split (self->xpos);
    tmp.combine (self->yank.copy ());
    tmp.combine (self->rivi);
    self->rivi = tmp;
    delete &tmp;

    self->drawRegion (self->xpos, -1, DRAW);
    self->gotoColumn (self->xpos);
    return (0);
}






#ifdef COMPLETE
/* simple file completion */

int     sortcmp(a, b)
    char  **a, **b;
{
    return (strcmp(*a, *b));
}


char  **make_list(char *line, int mode)
{
    struct comp_list 
    {
	char   *name;
	struct comp_list *next;
    };

#define new_comp_list() (struct comp_list *)malloc(sizeof(struct comp_list));

    static struct comp_list *p = NULL;
    struct comp_list *q;
    struct dirent *dp;
    DIR    *dire;
    char  **res = NULL, *path = ".", *file = NULL;
    int     len = strlen(line), i, j, nelems = 0;

    for (i = len - 1; (i >= 0) && (line[i] != ' '); i--)
	if ((line[i] == '/') && (!file)) {
	    file = line + i + 1;
	    line[i] = '\0';
	}
    i++;

    /* slash found => directory other than . */
    if (file)
	path = line + i;
    else
	file = line + i;

    len = strlen(file);

    /* now we should have separated dir and file */

    if (!(dire = opendir(path)))
	vout << "cannot open directory " << path << " !" << endv;

    if (*file != '.') {
	readdir(dire);	/* don't list . and .. files unless we want em */
	readdir(dire);
    }
    if (!p) {
	p = new_comp_list();
	p -> next = (struct comp_list *) (p -> name = NULL);
    }
    q = p;

    while (dp = readdir(dire)) {
	if (!strncmp(dp -> d_name, file, len)) {

	    /* free previous name */
	    /* if (q->name) free (q->name); */

	    q -> name = strdup(dp -> d_name + len * mode);

	    nelems++;
	    if (!q -> next) {
		q -> next = new_comp_list();
		q -> next -> next = (struct comp_list *) NULL;
		q -> next -> name = (char *) NULL;
	    }
	    q = q -> next;
	}
    }
    closedir(dire);

    /* convert linked list to a matrix */
    if (nelems) {
	res = (char **) malloc(sizeof (char *) * (nelems + 1));
	res[nelems] = NULL;
	for (q = p, i = 0; i < nelems; i++, q = q -> next) {
	    res[i] = q -> name;
	}
	qsort(res, nelems, sizeof (char *), sortcmp);
    }
    if (nelems == 1) {
	DIR    *dp;
	int     clen = strlen(res[0]);
	char   *only = malloc(strlen(path) + len + clen + 2);

	sprintf(only, "%s/%s%s", path, file, res[0]);
	res[0] = realloc(res[0], clen + 2);
	res[0][clen] = (dp = opendir(only)) ? '/' : ' ';
	res[0][clen + 1] = '\0';
	if (dp)
	    closedir(dp);
    }
    return (res);
}

int EditTask :: k_complete()
{
    int     i = 0, flag = 1, j;
    char    n, **list;

    if (!(list = complete_lister(strndup(rivi + minpos, xpos - minpos), 1))) {
	beep();
	return (0);
    }
    do {
	n = list[0][i];
	for (j = 1; list[j]; j++)
	    if (list[j][i] != n)
		flag = 0;
	i++;
    } while (flag && n);
    k_self_insert(list[0], i - 1);

    if (!flag)
	beep();

    /* free allocated memory */
    for (j = 0; list[j]; j++)
	free(list[j]);
    free(list);
    return (0);
}

int EditTask :: k_complete_list()
{
    char  **list = make_list(strndup(rivi + minpos, xpos - minpos), 0);
    char   *p;
    int     width = 0, i, j, k;

    for (j = 0; list[j]; j++)
	if ((i = strlen(list[j])) > width)
	    width = i;
    bufout(0x0d);
    bufout(0x0a);
    for (k = 0, j = 0; list[j]; j++) {
	for (i = 0, p = list[j]; i < width + 2; i++, k++)
	    if (*p)
		bufout(*p++);
	    else
		bufout(' ');

	if ((k + width > 78) || !list[j + 1]) {
	    bufout(0x0d);
	    bufout(0x0a);
	    bflush();
	    k = 0;
	}
	/* free allocated memory */
	free(list[j]);
    }
    free(list);
    k_redraw();
    return (0);
}


char   *strndup(char *str, int n)
{
    char   *t = (char *) malloc(n + 1);
    strncpy(t, str, n);
    t[n] = '\0';
    return (t);
}

#endif




//
// misc functions
//


int EditTask :: drawRegion (int start, int end, enum RegionCommand mod)
{
    int i, j = 0;

    FrameIterator iterator (&rivi);

    if (end == -1)
	end = (int) rivi.length ();

    for (i = start; i < end; i++) {
    
	switch (mod) {
	  case DRAW:
	    outbuf.outChar(iterator.read (i));
	    break;

	  case EMPTY:
	    for (j = charlen(iterator.read (i)); j; j--) {
		outbuf.outChar (' ');
	    }
	    break;

	  case COUNT:
	    j += charlen(iterator.read (i));
	    break;
	}
    }
    return (j);
}

void EditTask :: gotoColumn (FrameSize x)
{
    /* backward */
    while (x < outbuf.cursor) {
	outbuf.backChar (rivi.read (outbuf.cursor - 1));
    }

    /* forward */
    while (x > outbuf.cursor) {
	outbuf.outChar (rivi.read (outbuf.cursor));
    }

    outbuf.flush();
    return;
}

int     charlen(unsigned char ch)
{
    return ((ch < 0x20) || (ch > 0x7e) ? 2 : 1);
}

/*
char   *strdup(char *str)
{
    char   *t = (char *) malloc(strlen(str) + 1);
    strcpy(t, str);
    return (t);
}
*/

void    EditTask :: set_ckey(unsigned char ch, const char *funn)
{
    char    t[2];
    t[0] = ch;
    t[1] = '\0';
    e_bind_key(t, funn);
}

void    EditTask :: set_key (const char *str, unsigned int fun)
{
    int     i;
    switch (i = match_key(str, strlen(str))) {
      case 0:
	for (i = 0; i < MAXKEYS; i++)
	    if (!comlist[i].str)
		break;
	if (i == MAXKEYS) {
	    vout << "not enough room!" << endv;
	    return;
	}
	break;
      case -1:
	vout << "impossible!" << endv;
	return;
      default:
        if (comlist[i].str)
	    delete (comlist[i].str);
    }
    comlist[i].com = fun;
    
    comlist[i].str = new char [strlen(str)+1];
    memcpy (comlist[i].str, str, strlen(str)+1);
}

//
// OutputBuffer class
//
//
//

void EditTask :: OutputBuffer :: backChar (Byte ch)
{
    int i = charlen(ch);
    cursor --;
    while (i--) {
	outputBuffer.putLast (0x08);
    }
}

void EditTask :: OutputBuffer :: eraseChar (Byte ch)
{
    int i, len = charlen(ch);
    for (i = len; i; i--) {
	outputBuffer.putLast (' ');
    }
    for (i = len; i; i--) {
	outputBuffer.putLast (0x08);
    }
}

void EditTask :: OutputBuffer :: outChar(Byte ch)
{
    cursor++;
    if (ch < 0x20) {
	outputBuffer.putLast ('^');
	ch |= 0x40;
    }
    if (ch > 126) {
	outputBuffer.putLast ('^');
	ch = '?';
    }
    outputBuffer.putLast (ch);
}

void EditTask :: OutputBuffer :: outString(const Byte *str)
{
    for (const Byte *p = str; *p; p++) {
	outChar(*p);
    }
}

void EditTask :: OutputBuffer :: flush(void)
{
    FrameSize outcount = outputBuffer.length ();
    Byte *buffer = (Byte *)outputBuffer;
    ::write (fd, (const char *) buffer, (int) outcount);
    delete buffer;
    outputBuffer.destroy ();
}

void EditTask :: OutputBuffer :: beep (void)
{
    outputBuffer.putLast ('\07');
    flush ();
}

void EditTask :: OutputBuffer :: eol (void)
{
    outputBuffer.putLast ('\r');
    outputBuffer.putLast ('\n');
    flush ();
    cursor = 0;
}

struct k_func funlist[] = {

    {	"no-operation", 	EditTask::k_nop    },
    {	"self-insert-key", 	EditTask::k_self_insert    },
    {	"previous-history", 	EditTask::k_prev_history    },
    {	"next-history", 	EditTask::k_next_history    },
    {	"forward-char", 	EditTask::k_forward    },
    {	"backward-char", 	EditTask::k_backward    },
    {	"backspace", 		EditTask::k_backspace    },
    {	"end-of-line", 		EditTask::k_eol    },
    {	"end-of-file", 		EditTask::k_eof    },
    {	"goto-end", 		EditTask::k_end    },
    {	"goto-begin", 		EditTask::k_begin    },
    {	"redraw-line", 		EditTask::k_redraw    },
    {	"delete-char", 		EditTask::k_delete    },
    {	"kill-to-begin", 	EditTask::k_kill_begin    },
    {	"kill-to-end", 		EditTask::k_kill_end    },
    {	"yank", 		EditTask::k_yank    },

#ifdef COMPLETE
    {	"complete-file", 	EditTask::k_complete    },
    {	"list-complete", 	EditTask::k_complete_list    },
#endif

    {  0 , 0    }
};









