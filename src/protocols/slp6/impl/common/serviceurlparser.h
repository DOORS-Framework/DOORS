
#include <string>
#include <map>

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

class DOORS_ServiceURL_Parser_Class {

public:

     DOORS_ServiceURL_Parser_Class();
     ~DOORS_ServiceURL_Parser_Class();

     int yylex();
     void setURL(std::string);

     struct Span {
          int a,b;
     };

     void addAttribute(struct Span a, struct Span b);

     void setAddrSpec(struct Span a);
     std::string getAddrSpec(void);
     void setAbstractType(struct Span a);
     void setPureType();
     bool isPureType() { return serviceIsPureType_; };
     std::string getAbstractType(void);
     void setConcreteType(struct Span a);
     std::string getConcreteType(void);
     void setHostAddr(struct Span a);
     std::string getHostAddr(void);
     void setPort(struct Span a);
     int getPort(void);
     void setTransient();
     bool getTransient();
     std::map<std::string,std::string> getAllAttributes();
     
private:
     
     std::string url;

     std::map<std::string,std::string> attributes_;

     std::string service_; // service-type, abstract service name
     std::string access_; // url scheme, or "" if service_ is concrete
     std::string address_;
     std::string host_;
     int port_;
     
     unsigned int pos;

     bool isTransient;
     bool serviceIsPureType_;
};
