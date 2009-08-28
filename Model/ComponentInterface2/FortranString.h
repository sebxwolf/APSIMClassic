#ifndef FortranStringH
#define FortranStringH

#include <string.h>
#include <vector>
#include <stdexcept>
#include <General/platform.h>
class MessageData;

// ------------------------------------------------------------------
// Encapsulates a case insensitive FORTRAN string.
// ------------------------------------------------------------------
class EXPORT FortranString
   {
   public:
      static const unsigned npos = (unsigned) -1;

      FortranString(void)
         {
         text = NULL;
         len = 0;
         realLen = len;
         canModify = true;
         }
      // constructor for an alias to a FORTRAN string
      FortranString(char* t, unsigned int tLength)
         {
         text = t;
         len = tLength;
         realLen = len;
         canModify = true;
         calcRealLength();
         }
      // constructor for an alias to a FORTRAN string
      FortranString(const char* t, unsigned int tLength)
         {
         text = (char*) t;
         len = tLength;
         realLen = len;
         canModify = true;
         calcRealLength();
         }
      // copy constructor
      FortranString(const FortranString& rhs)
         {
         text = rhs.text;
         canModify = rhs.canModify;
         len = rhs.len;
         realLen = rhs.realLen;
         }

      bool operator== (const FortranString& rhs) const
         {
         #if __WIN32__
         return (length() == rhs.length() &&
                 strncmpi(f_str(), rhs.f_str(), length()) == 0);
         #else
         return (length() == rhs.length() &&
                 strncasecmp(f_str(), rhs.f_str(), length()) == 0);
         #endif
         }
      bool operator!= (const FortranString& rhs) const
         {return !(*this == rhs);}
      char operator[] (unsigned index) {return text[index];}
      const char* f_str(void) const
         {
         return text;
         }
      std::string toString() const
         {
         return std::string(text, length());
         }
      FortranString& operator= (const FortranString& rhs)
         {
         unsigned int rhsLength = rhs.length();
         if (len < rhsLength)
            throw std::runtime_error(std::string("String truncation.  FORTRAN string not long enough\n") +
                                     "to hold the string:\n" + rhs.toString());
         else
            {
            memcpy(text, rhs.f_str(), rhsLength);
            memset(&text[rhsLength], ' ', len - rhsLength);
            realLen = rhsLength;
            }
         return *this;
         }
      FortranString& operator= (const std::string& rhs)
         {
         unsigned int rhsLength = rhs.length();
         if (len < rhsLength)
            throw std::runtime_error(std::string("String truncation.  FORTRAN string not long enough\n") +
                                     "to hold the string:\n" + rhs);
         else
            {
            memcpy(text, rhs.c_str(), rhsLength);
            memset(&text[rhsLength], ' ', len - rhsLength);
            realLen = rhsLength;
            }
         return *this;
         }
      unsigned int length(void) const
         {
         return realLen;
         }
      unsigned int maxLength(void) const
         {
         return len;
         }
      FortranString substr(unsigned int pos, unsigned int nchar = npos) const
         {
         if (nchar == npos)
            nchar = length() - pos;
         else if (pos+nchar > len)
            throw std::runtime_error("Invalid index into string: " + toString());
         return FortranString(&text[pos], nchar);
         }
   private:
      char* text;
      bool canModify;
      unsigned int len;
      unsigned int realLen;

      void calcRealLength(void)
         {
         for (realLen = len; realLen > 0 && text[realLen-1] == ' '; realLen--);
         }
   };

// ------------------------------------------------------------------
// Encapsulates a list of FORTRAN strings.
// ------------------------------------------------------------------
class EXPORT FortranStrings
   {
   public:
      FortranStrings(char* t,
               const unsigned int elementlength,
               const unsigned int maxNumElement,
               unsigned int numElement)
         : maxNumElements(maxNumElement),
           numElements(numElement),
           elementLength(elementlength),
           st(t, elementlength*maxNumElements)
           {
           }

      template <class CT>
      FortranStrings& operator= (const CT& strings)
         {
         unsigned pos = 0;
         numElements = 0;
         if (strings.size() > maxNumElements)
            throw std::runtime_error("Too many strings for FORTRAN string array");
         for (typename CT::const_iterator i = strings.begin();
                                 i != strings.end();
                                 i++)
            {
            FortranString rhs((char*) i->c_str(), i->length());
            st.substr(pos, elementLength) = rhs;
            pos += elementLength;
            numElements++;
            }
         return *this;
         }

      FortranString getString(unsigned index) const
         {
         return st.substr(index * elementLength, elementLength);
         }
      void addString(FortranString& st)
         {
         getString(numElements) = st;
         numElements++;
         }
      void addString(const std::string& st)
         {
         getString(numElements) = st;
         numElements++;
         }

      unsigned getNumElements(void) const {return numElements;}
      unsigned getElementLength(void) const {return elementLength;}
   private:
      unsigned maxNumElements;
      unsigned numElements;
      unsigned elementLength;
      FortranString st;
   };


void EXPORT unpack(MessageData& messageData, FortranString& value);
void EXPORT pack(MessageData& messageData, const FortranString& value);
unsigned EXPORT memorySize(const FortranString& value);
std::string EXPORT DDML(const FortranString& );
void EXPORT Convert(bool source, FortranString& dest);
void EXPORT Convert(int source, FortranString& dest);
void EXPORT Convert(const std::string& source, FortranString& dest);
void EXPORT Convert(float source, FortranString& dest);
void EXPORT Convert(double source, FortranString& dest);
//template <class FROM, class TO>
inline void Convert(const std::vector<std::string>& from, FortranString& to)
   {
   std::string st;
   st = "";
   for (unsigned i = 0; i != from.size(); i++)
      {
      if (st != "")
         st += " ";
      st += from[i];
      }
   to = st.c_str();
   }

void EXPORT unpack(MessageData& messageData, FortranStrings& value);
void EXPORT pack(MessageData& messageData, const FortranStrings& value);
unsigned EXPORT memorySize(const FortranStrings& value);
std::string EXPORT DDML(const FortranStrings& );
void EXPORT Convert(bool source, FortranStrings& dest);
void EXPORT Convert(int source, FortranStrings& dest);
void EXPORT Convert(const std::string& source, FortranStrings& dest);
void EXPORT Convert(const std::vector<std::string>& source, FortranStrings& dest);
void EXPORT Convert(float source, FortranStrings& dest);
void EXPORT Convert(double source, FortranStrings& dest);
#endif
