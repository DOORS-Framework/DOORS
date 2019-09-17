
//  Copyright 2001
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND 
//  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY 
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


// LDAP Browser
// very ugly, but it works


#include <iostream>
#include <vector>
#include <string>
#include <lber.h>
#include <ldap.h>

#include <doors/ldapcon.h>
#include <doors/ldapattributelist.h>

#define LDAPDefault 42

#define LDAPDefaultHost "woodstock.cs.tut.fi"
#define LDAPDefaultPort LDAPConnection::oma_default_port;
#define LDAPDefaultBindUser "cn=Manager,o=CS,c=FI"
#define LDAPDefaultBindPass "doors"
#define LDAPDefaultBase "o=CS,c=FI"
#define LDAPDefaultFilter "(objectClass=*)"

#define HELP_CMD "help" // get help
#define EXIT_CMD "exit" // quit program
#define OPEN_CMD "open" // connect to server
#define CLOSE_CMD "unbind" // logoff
#define BIND_CMD "bind" // bind == log in
#define BASE_CMD "base" // define base
#define BACK_CMD "back" // go to previous base
#define GOTO_CMD "goto" // go to subentry  
#define LIST_CMD "list" // list subentries
#define HIST_CMD "hist" // list base history
#define READ_CMD "read" // read values of current entry
#define FIND_CMD "find" // find entries
#define REPL_CMD "repl" // replace attribute values
#define DEL_CMD "delete" // delete entries or attributes
#define ADD_CMD "add"   // add a new entry with initial 
#define NEW_CMD "_._._._" // 



/*

bool split_cmd(string inp,vector<string> &outp,int &count);

Takes a string input, parses the string so, that different
agruments separated by spaces or tabs are placed in vector
as individual elements. Arguments placed in quotes "" are
stored as-is, so any spaces will be passed on.
Returns a boolean result indicating whether everything went successfully
or not.

*/

bool split_cmd(std::string inp,std::vector<std::string> &outp,int &count)
{
  
  bool inquote = false;
  bool toomuch = false;
  int arg_c = 0;
  int chr_c = 0;
  int c_ptr = 0;
  //  int a_ptr = 0;
  std::string tmps;
  int ilen = inp.length();
  
  tmps = "";
  count = 0;
  
  outp.clear();

  if ( ilen > 0 )  // if there's something to process
    {
      for ( c_ptr = 0; c_ptr < ilen ; c_ptr++ ) // go thru all characters
	{
	  
	  switch  ( inp[c_ptr] ) { // and with every character


	  case '"':   // make note if we are in quotes or not
	    inquote = !inquote;
	    break;
	    
	  case '\t': // tabs and spaces separate args
	  case ' ':
	    if (!inquote) // if not in quotes
	      {
		if (chr_c > 0) // and there was some chars 
		  {
		    // if( arg_c < MAX_ARGS ) // and room for a new arg
		      {
			
			outp.push_back(tmps); // put the new arg
			chr_c = 0;        // zero the char counter
			arg_c++;          // advance to next arg
			tmps = "";        // zero the tempstring
		      }
		      // else toomuch = true;

		  }
	      } else { // if in quotes, do same as with every char
		tmps=tmps+inp[c_ptr]; // store spaces
		chr_c++;  // increase counter
	      }
	    break;
	    
	  default: // normal chars are added to args
	    
	    tmps=tmps+inp[c_ptr];  // store characters
	    chr_c++;  // increase counter

	    break;
	  }
	  
	  
	}

      // input ended, process what's left in temp
      
      if (!inquote)  // if not in quotes, good thing
	if (chr_c > 0) // if chars to process
	  {
	    // if( arg_c < MAX_ARGS ) // if room in buffer
	      {
		
		outp.push_back(tmps); // store arg
		chr_c = 0; // actually these are just copied
		arg_c++;  // from above, but not actually needed :)
		tmps = "";
	      }
	      // else toomuch = true;
	  }
      
      
    }
  
  count = arg_c; // return argument count
  
  if ( toomuch ) {  // if too many arguments, parse error
    return false;  // because there might be important parameters missing
  }

  return !inquote; // parse error if no ending quote anywhere
  
}


void help_proc(void)
{

  std::cout << "Commands:" << std::endl;


  std::cout << "OPEN server [port] (connects to a server)" << std::endl;
  std::cout << "BIND [dn [passwd]] (login, also anonymous)" << std::endl;
  std::cout << "BASE [dn] (shows/[sets] list and search base)" << std::endl;
  std::cout << "LIST (lists subentries, must use before goto)" << std::endl;
  std::cout << "GOTO n (sets base dn to selected subentry, use list first)" << std::endl;
  std::cout << "READ (shows attributes of current entry)" << std::endl;
  std::cout << "FIND base scope filter [attr ...] (search, type find for more help) " 
       << std::endl;

  std::cout << "ADD dn attr val [attr val ... ...] (adds an entry)" << std::endl;
  std::cout << "DELETE dn [attr [val]] (deletes an entry or selected attribute(s))" 
       << std::endl;

  std::cout << "REPL dn attr newval (replace)" << std::endl;
  std::cout << "HIST (shows base dn history)" << std::endl;
  std::cout << "BACK (sets previous base dn)" << std::endl;

  std::cout << "UNBIND (disconnects from a server)" << std::endl;

  std::cout << "HELP shows this help" << std::endl;
  std::cout << "EXIT quits program" << std::endl;

}

/*
bool open_proc(LDAPConnection **ld, , int count)
{

  *ld = new LDAPConnection( host,
			    LDAPConnection::oma_default_port );



}
*/

bool exit_proc(LDAPConnection *ld)
{



  //  std::cout << ld << std::endl;

  if ( ld != NULL )
    {

      std::cout << "Closing connection" << std::endl;
      delete ld;
    }

  std::cout << "BYE!" << std::endl;

  return true;

}

int main (void)
{ 

  std::string cmdline;
  bool endflag = false;
  std::vector<std::string> params;
  int paramcnt;


  // ldap specific variables

  std::string LDAPHost = LDAPDefaultHost;
  int LDAPPort = LDAPDefaultPort; 

  std::string LDAPBindUser = LDAPDefaultBindUser;
  std::string LDAPBindPass = LDAPDefaultBindPass;

  std::string LDAPBase = LDAPDefaultBase;
  
  int LDAPScope = 0;

  std::string LDAPFilter = LDAPDefaultFilter;
  LDAPAttributelist LDAPAttrlist;
  std::vector<std::string> LDAPAttrs;

  LDAPConnection *ld = NULL;

  std::vector<std::string> subentries;
  int subentrycount=0;
  std::vector<std::string> dirstack;

  //std::vector<std::string> LDAPAttrNames, LDAPAttrValues;

  while ( ! endflag )

    {

      //      cin.getline() >> cmdline;

      std::cout << std::endl << "--> " ;

      getline(std::cin,cmdline);

      std::cout << std::endl;

      if ( std::cin ) {
	      
	//	std::cout << cmdline << std::endl;

	if ( split_cmd( cmdline, params, paramcnt) )
	  {

	    if (paramcnt>0){
	  


	      /*
		std::cout << "split " << paramcnt  << std::endl;
		
	      for ( int i = 0; i < paramcnt ; i++ )
	      {
	      
	      std::cout << i << "(" << params[i].length() 
		       << "):" << params[i] << std::endl;
		       
		       }
		       
	      */
	      

	      cmdline = params[0];
	      
	      if (cmdline == HELP_CMD) 
		{

		  help_proc();
 
		} 
	      else


	      if (cmdline == NEW_CMD)
		{} else

		  
	      if (cmdline == OPEN_CMD)
		{
		  
		  if ( (paramcnt > 3))
		    {
		      std::cout << "OPEN [HOST [PORT]]" << std::endl;
		    }
		  else
		    {

		      if (paramcnt > 1)
			{ 
			  LDAPHost = params[1];
			}
		      else
			{
			  std::cout << LDAPHost << std::endl;
			}

		      if (paramcnt == 3) 
			{
			  LDAPPort = atoi(params[2].c_str());
			} /* else {
			     LDAPPort = LDAPConnection::oma_default_port;
			     }*/
		      		      
		      ld = new LDAPConnection( LDAPHost,LDAPPort );

		      std::cout << ld << std::endl ;

		      if ( ld != NULL ) {
			std::cout << "Connection established" << std::endl;
		      }

		    }
		   
		} else

	      if (cmdline == BIND_CMD)
		{

		  if (paramcnt < 4 )
		    {
		      if ( ld != NULL )
			{
			  if (paramcnt > 1 )
			    {
			      LDAPBindUser = params[1];
			    }
			  if (paramcnt > 2 )
			    {
			      LDAPBindPass = params[2];
			    }

			  if ( ld->bind( LDAPBindUser, LDAPBindPass ) !=LDAP_SUCCESS)
			    {
			      std::cout << "bind failed" << std::endl;
			    }
			  else
			    {
			      std::cout << "bind successful" << std::endl;
			    }
			} 
		      else
			{
			  std::cout << "Connection not open" << std::endl;
			}
		    }
		  else
		    {
		      std::cout << "BIND [dn [passwd]]" << std::endl;
		    }
		  

		} 
	      else


	      if (cmdline == BASE_CMD)
		{

		  if (paramcnt == 1)
		    {
		      std::cout << "\"" << LDAPBase << "\"" << std::endl;
		    }
		  else
		    {
		      dirstack.push_back(LDAPBase);
		      LDAPBase = params[1];

		    }

		} 
	      else


	      if (cmdline == LIST_CMD)
		{

		  //		  if ( ld != NULL )
		  //{
		 
		      LDAPFilter = "(objectClass=*)";
		      LDAPAttrs.clear();
		      subentries.clear();
		      subentrycount = 0;

		      int sr_ = 0;

		      sr_ = ld->search( LDAPBase, LDAP_SCOPE_ONELEVEL,
					LDAPFilter, LDAPAttrs, 0  ) ;

		      if ( sr_ != 0 )

			{
			  std::cout << "Search error " << sr_ << std::endl;
			}
		      else
			{
			  
			  if ( ld->first_entry() == false )
			    {
			      std::cout << "No subentries found" << std::endl;
			    }
			  else
			    {
			      
			      bool P=true;
			      for ( P=ld->first_entry() ; P; P=ld->next_entry() )
				{
				  subentrycount++;
				  subentries.push_back( ld->get_dn() );
				  
				}
			      
			      std::cout << "Found " << subentrycount << " subentries" << std::endl;
			      
			      for ( int i = 0; i < subentrycount; i++) {
				
				std::cout << "(" <<i+1 << ") \""
				     << subentries[i] << "\""<< std::endl ;
				
			      }
			      
			    }
			}
		      //	}
		  
		}
	      else

	      if (cmdline == GOTO_CMD)
		{

		  if (paramcnt != 2)
		    {

		      std::cout << "invalid subentry" << std::endl;

		    }
		  else
		    {

		      int i = atoi(params[1].c_str());

		      if (i <= subentrycount) 
			{
			  if (i == 0) 
			    {
			      std::cout << "not yet implemented ;-)" << std::endl;
			    } 
			  else
			    {
			      dirstack.push_back(LDAPBase);
			      LDAPBase = subentries[i-1];
			    }
			}
		      else
			{
			  std::cout << "invalid subentry" << std::endl;
			}

		    }

		} 
	      else

	      if (cmdline == READ_CMD)
		{

		  //LDAPAttrNames.clear();
		  //LDAPAttrValues.clear();
		  LDAPAttrs.clear();

		  if (ld->search( LDAPBase, LDAP_SCOPE_BASE, LDAPFilter,
				  LDAPAttrs, 0  ) != 0 )
		    {
		      std::cout << "Search error" << std::endl;
		    }
		  else
		    {
		      
		      if ( ld->first_entry() == false )
			{
			  std::cout << "No base found" << std::endl;
			}
		      else
			{

			  std::cout << "dn: \"" << ld->get_dn() << "\"" << std::endl;

			  std::string tmpst1 = "";
			  tmpst1 = ld->first_attribute();
			  while ( tmpst1 != "" )
			    {

			      std::string tmpst2 = "";
			      tmpst2 = ld->first_value();

			      while ( tmpst2 != "" ) {
				

				std::cout << tmpst1 << ": \"" << tmpst2 << "\""<< std::endl;
				
				tmpst2 = ld->next_value();

			      }

			      tmpst1 = ld->next_attribute();

			    }


			}
		      
		    }
		  
		} 
	      else

	      if (cmdline == FIND_CMD)
		{

		  if (paramcnt < 4)
		    {
		      std::cout << "find base scope filter [attr ...]" << std::endl;
		      std::cout
			<< "search for dn entry scope deep at given base with" << std::endl
			<< "selected filter showing all or only given attributes" << std::endl
			<< "scope is \"sub\" for whole subtree "
			<< "or \"one\" for one sublevel"  
			<< std::endl;
		    }
		  else
		    {

		      LDAPAttrs.clear();
		      
		      if (paramcnt>4)
			{
			  for (int i = 4; i<paramcnt ; i++)
			    {
			      //   std::cout << i << params[i] << paramcnt << std::endl;
			      LDAPAttrs.push_back(params[i]);
			      // std::cout << "dbg-1"<< std::endl;
			    }
			  //std::cout << "dbg0"<< std::endl;
			}
		      //std::cout << "dbg1"<< std::endl;

		      

		      LDAPBase = params[1];
		      LDAPFilter = params[3];

		      if ( params[2] == "sub" )
			{
			  LDAPScope = LDAP_SCOPE_SUBTREE;
			}
		      else
			if ( params[2] == "one" )
			{
			  LDAPScope = LDAP_SCOPE_ONELEVEL;
			}
		      else
			{
			  std::cout << "invalid scope" << std::endl ;
			  LDAPScope = -1;
			}

		      //		         std::cout << "dbg2"<< std::endl;
	
		      //if (1)
		      if (ld->search( LDAPBase, LDAPScope, LDAPFilter,
				      LDAPAttrs, 0  ) != 0 )
			{
			  std::cout << "Search error" << std::endl;
			}
		      else
			{
			  //			  std::cout << "dbg3"<< std::endl;

			  if ( ld->first_entry() == false )
			    {
			      std::cout << "No base found" << std::endl;
			    }
			  else
			    {
			      bool P = true;

			      while ( P )
				{
				  
				  std::cout << "dn: \"" << ld->get_dn() << "\"" << std::endl;
				  
				  std::string tmpst1 = "";
				  tmpst1 = ld->first_attribute();
				  while ( tmpst1 != "" )
				    {
				      
				      std::string tmpst2 = "";
				      tmpst2 = ld->first_value();
				  
				      while ( tmpst2 != "" ) {
				    
					
					std::cout << tmpst1 << ": \"" 
					     << tmpst2 << "\""<< std::endl;
					
					tmpst2 = ld->next_value();
					
				      }
				      
				      tmpst1 = ld->next_attribute();
				      
				    }

				  std::cout << std::endl;
				  
				 
				  P = ld->next_entry();

				}
			      
			    }
			  
			  
			}
		    }
		}
	      else



	      if (cmdline == REPL_CMD)
		{
		  
		  if ( paramcnt >3 )
		    {
		      int rv = ld->replace(params[1],params[2],params[3]);
		      if ( rv == LDAP_SUCCESS )
			{
			  std::cout << "successful" << std::endl;
			}
		      else
			{
			  std::cout << rv <<"failed" << std::endl;
			}

		    }
		  else
		    {
		      std::cout << "syntax error" << std::endl;
		    }
		}
	      else


	      if ( cmdline == DEL_CMD )
		{
		  int rv = LDAP_SUCCESS;
		  bool P=true;

		  if (paramcnt==2) // del whole entry
		    {
		      rv = ld->del(params[1]);
		    }
		   else

		  if (paramcnt==3) // del all given attributes 
		    {
		      rv = ld->del(params[1],params[2]);
		    }
		   else

		  if (paramcnt==4) // del given attr with given val
		    {

		      //  std::cout << params[1] << std::endl << params[2] << std::endl
		      //       << params[3] << std::endl ;
		      rv = ld->del(params[1],params[2],params[3]);
		    }
		   else
		     {
		       P=false;
		       std::cout << "delete dn [attr [val]]" << std::endl; 
		     }

		  if (P)
		    if ( (rv == LDAP_SUCCESS))
		      {
			std::cout << "deleted" << std::endl;
		      }
		    else
		      {
			std::cout << rv <<" failed" << std::endl;
		      }
			
		}
	      else


	      if (cmdline == ADD_CMD)
		{

		  if ( paramcnt > 3 ) {

		    //LDAPConnection::LDAPAttribute AddRec;
		    //std::vector<LDAPConnection::LDAPAttribute> AddStruct;
			LDAPAttributelist attributes;
		    
		    //AddStruct.clear();

		    if ( (paramcnt%2) == 0 )
		      {

			for (int i=2; i<paramcnt; i +=2)
			  {
			    std::cout << params[i] << ": " << params[i+1] << std::endl;
			    //AddRec.name=params[i];
			    //AddRec.value=params[i+1];
			    //AddStruct.push_back(AddRec);
				attributes.addAttribute(params[i], params[i+1]);
			  }

			int rv=0;
		        rv=ld->add_entry(params[1],attributes);
			if (rv == LDAP_SUCCESS )
			  {
			    std::cout << "completed" << std::endl;
			  }
			else
			  {
			    std::cout << rv << "failed" << std::endl;
			  }
		      }
		    else
		      {
			std::cout << "odd number of parameters" << std::endl;
		      }
		  }	
		else
		  {
		    std::cout << "add dn attr val [attr val ... ...]" << std::endl;
		  }

		  
		}
	      else
		    
	      if (cmdline == BACK_CMD)
		{

		  if (dirstack.size() > 0 )
		    {
		      LDAPBase = dirstack[dirstack.size()-1]; // Haetaan edellinen taso
		      dirstack.pop_back();    // Poistetaan se listasta
		    }
		  else
		    {
		      std::cout << "no base history" << std::endl;
		    }
		} 
	      else

		if (cmdline == HIST_CMD)
		{

		  if (dirstack.size()>0)
		    {
		      for ( unsigned int i = 0; i < dirstack.size(); i++)
			{
			  std::cout << "\"" << dirstack[i] << "\"" << std::endl;
			}
		    }
		  else
		    {
		      std::cout << "no base history" << std::endl;
		    }

		} 
	      else


	      if (cmdline == CLOSE_CMD)
		{

		  std::cout << ld << std::endl ;
		  
                  if ( ld != NULL )
                    {
                      delete ld;
                      std::cout << "Connection closed" << std::endl;
		      ld = NULL;
                    }

		} 
	     else


	      if (cmdline == EXIT_CMD)
		{

		  if ( ld != NULL )
		    {
		      std::cout << "Connection still open" << std::endl;
		    }
		  else
		    {

		      
		      std::cout << "BYE!" << std::endl;
		      
		      endflag = true;
		    }
		} 
	       else
	       {
		  
		 std::cout << "Bad command" << std::endl;
		  
	       }
	      
	      
	    }
          else
	    {
	      help_proc();
	    }
	  } else {

	    std::cout << "Parse error!" << std::endl;
	    
	  }

	
      } else { 
	
	std::cout << "CTRL-D pressed, exiting..." << std::endl;

	std::cout << ld << std::endl ;
	
	if ( ld != NULL )
	  {
	    std::cout << "Connection still open" << std::endl;
	    delete ld;
	    std::cout << ld << std::endl ;
	    std::cout << "Connection closed" << std::endl;
	  }

	
	endflag = true;

      }


    }

  return EXIT_SUCCESS;
}







