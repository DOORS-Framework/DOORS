//Editor-Info: -*- C++ -*-
//


#ifndef ALIAS_H
#define ALIAS_H

#include <doors/name.h>
#include <doors/otl_dcl.h>
#include <doors/tqueue.h>

//
//Class: AliasList
//
//Description: 
//

class AliasList {
  public:
    AliasList (void);
    virtual ~AliasList (void);

    WordList *get (const Name *n);
    bool add (Name *n, WordList *replacement);
    bool remove (const Name *n);
    void removeAll (void);

    void print (void) const;

  private:
    TQueue<Alias> aliases;
};


class Alias {
  public:
    Alias (Name *a, WordList *replacement);
    virtual ~Alias (void);
    
    Name *getName (void);
    WordList *getReplacement (void);

    void print (void) const;

    bool operator == (const Name *n);
    bool operator == (Alias &a);

  private:
    Name alias;
    WordList replacement;
};

#endif  // ALIAS_H

