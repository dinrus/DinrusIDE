#include "tinyxml.h"
namespace UPP{

  //  TiXmlString (const char* instring);

    TiXmlString::TiXmlString ()
    {
        allocated = 0;
        cstring = NULL;
        current_length = 0;
    }

   // TiXmlString::TiXmlString (const TiXmlString& copy);

    TiXmlString::~TiXmlString ()
    {
        empty_it ();
    }

    const char* TiXmlString::c_str () const
    {
        if (allocated)
            return cstring;
        return "";
    }

    unsigned TiXmlString::length () const
	{
		return ( allocated ) ? current_length : 0;
	}

    // TiXmlString = operator
  //  void operator = (const char* content);

    // = operator
   // void operator = (const TiXmlString & copy);

    // += operator. Maps to append
    TiXmlString& TiXmlString::operator += (const char* suffix)
    {
        append (suffix);
		return *this;
    }

    // += operator. Maps to append
    TiXmlString& TiXmlString::operator += (char single)
    {
        append (single);
		return *this;
    }

    // += operator. Maps to append
    TiXmlString& TiXmlString::operator += (TiXmlString & suffix)
    {
        append (suffix);
		return *this;
    }
   // bool operator == (const TiXmlString & compare) const;
   // bool operator < (const TiXmlString & compare) const;
  //  bool operator > (const TiXmlString & compare) const;

    bool TiXmlString::empty () const
    {
        return length () ? false : true;
    }

    const char& TiXmlString::at (unsigned index) const
    {
        assert( index < length ());
        return cstring [index];
    }

    unsigned TiXmlString::find (char lookup) const
    {
        return find (lookup, 0);
    }

    // find a char in a generic_string from an offset. Return TiXmlString::notfound if not found
    //unsigned find (char tofind, unsigned offset) const;

    void TiXmlString::reserve (unsigned size)
    {
        empty_it ();
        if (size)
        {
            allocated = size;
            cstring = new char [size];
            cstring [0] = 0;
            current_length = 0;
        }
    }

    // [] operator
    char& TiXmlString::operator [] (unsigned index) const
    {
        assert( index < length ());
        return cstring [index];
    }

   // void append (const char*str, int len );

    unsigned TiXmlString::assign_new_size (unsigned minimum_to_allocate)
    {
        return minimum_to_allocate * 2;
    }

    void TiXmlString::empty_it ()
    {
        if (cstring)
            delete [] cstring;
        cstring = NULL;
        allocated = 0;
        current_length = 0;
    }

   // void TiXmlString::append (const char*suffix );

    void TiXmlString::append (const TiXmlString & suffix)
    {
        append (suffix . c_str ());
    }

    void TiXmlString::append (char single)
    {
        char smallstr [2];
        smallstr [0] = single;
        smallstr [1] = 0;
        append (smallstr);
    }


// TiXmlString constructor, based on a C generic_string
TiXmlString::TiXmlString (const char* instring)
{
    unsigned newlen;
    char* newstring;

    if (!instring)
    {
        allocated = 0;
        cstring = NULL;
        current_length = 0;
        return;
    }
    newlen = strlen (instring) + 1;
    newstring = new char [newlen];
    memcpy (newstring, instring, newlen);
    allocated = newlen;
    cstring = newstring;
    current_length = newlen - 1;
}

// TiXmlString copy constructor
TiXmlString::TiXmlString (const TiXmlString& copy)
{
    unsigned newlen;
    char* newstring;

    // Prevent copy to self!
    if ( &copy == this )
        return;

    if (! copy . allocated)
    {
        allocated = 0;
        cstring = NULL;
        current_length = 0;
        return;
    }
    newlen = copy . length () + 1;
    newstring = new char [newlen];
    memcpy (newstring, copy . cstring, newlen);
    allocated = newlen;
    cstring = newstring;
    current_length = newlen - 1;
}

// TiXmlString = operator. Safe when assign own content
void TiXmlString ::operator = (const char* content)
{
    unsigned newlen;
    char* newstring;

    if (! content)
    {
        empty_it ();
        return;
    }
    newlen = strlen (content) + 1;
    newstring = new char [newlen];
    memcpy (newstring, content, newlen);
    empty_it ();
    allocated = newlen;
    cstring = newstring;
    current_length = newlen - 1;
}

// = operator. Safe when assign own content
void TiXmlString ::operator = (const TiXmlString & copy)
{
    unsigned newlen;
    char* newstring;

    if (! copy . length ())
    {
        empty_it ();
        return;
    }
    newlen = copy . length () + 1;
    newstring = new char [newlen];
    memcpy (newstring, copy . c_str (), newlen);
    empty_it ();
    allocated = newlen;
    cstring = newstring;
    current_length = newlen - 1;
}


//// Checks if a TiXmlString contains only whitespace (same rules as isspace)
//bool TiXmlString::isblank () const
//{
//    char* lookup;
//    for (lookup = cstring; * lookup; lookup++)
//        if (! isspace (* lookup))
//            return false;
//    return true;
//}

// append a const char* to an existing TiXmlString
void TiXmlString::append( const char* str, int len )
{
    char* new_string;
    unsigned new_alloc, new_size, size_suffix;

    size_suffix = strlen (str);
    if (len < (int) size_suffix)
        size_suffix = len;
    if (! size_suffix)
        return;

    new_size = length () + size_suffix + 1;
    // check if we need to expand
    if (new_size > allocated)
    {
        // compute new size
        new_alloc = assign_new_size (new_size);

        // allocate new buffer
        new_string = new char [new_alloc];
        new_string [0] = 0;

        // copy the previous allocated buffer into this one
        if (allocated && cstring)
            memcpy (new_string, cstring, length ());

        // append the suffix. It does exist, otherwize we wouldn't be expanding
        // strncat (new_string, str, len);
        memcpy (new_string + length (),
                str,
                size_suffix);

        // return previsously allocated buffer if any
        if (allocated && cstring)
            delete [] cstring;

        // update member variables
        cstring = new_string;
        allocated = new_alloc;
    }
    else
    {
        // we know we can safely append the new generic_string
        // strncat (cstring, str, len);
        memcpy (cstring + length (),
                str,
                size_suffix);
    }
    current_length = new_size - 1;
    cstring [current_length] = 0;
}


// append a const char* to an existing TiXmlString
void TiXmlString::append( const char* suffix )
{
    char* new_string;
    unsigned new_alloc, new_size;

    new_size = length () + strlen (suffix) + 1;
    // check if we need to expand
    if (new_size > allocated)
    {
        // compute new size
        new_alloc = assign_new_size (new_size);

        // allocate new buffer
        new_string = new char [new_alloc];
        new_string [0] = 0;

        // copy the previous allocated buffer into this one
        if (allocated && cstring)
            memcpy (new_string, cstring, 1 + length ());

        // append the suffix. It does exist, otherwize we wouldn't be expanding
        memcpy (new_string + length (),
                suffix,
                strlen (suffix) + 1);

        // return previsously allocated buffer if any
        if (allocated && cstring)
            delete [] cstring;

        // update member variables
        cstring = new_string;
        allocated = new_alloc;
    }
    else
    {
        // we know we can safely append the new generic_string
        memcpy (cstring + length (),
                suffix,
                strlen (suffix) + 1);
    }
    current_length = new_size - 1;
}

// Check for TiXmlString equuivalence
//bool TiXmlString::operator == (const TiXmlString & compare) const
//{
//    return (! strcmp (c_str (), compare . c_str ()));
//}

//unsigned TiXmlString::length () const
//{
//    if (allocated)
//        // return strlen (cstring);
//        return current_length;
//    return 0;
//}


unsigned TiXmlString::find (char tofind, unsigned offset) const
{
    char* lookup;

    if (offset >= length ())
        return (unsigned) notfound;
    for (lookup = cstring + offset; * lookup; lookup++)
        if (* lookup == tofind)
            return lookup - cstring;
    return (unsigned) notfound;
}


bool TiXmlString::operator == (const TiXmlString & compare) const
{
    if ( allocated && compare.allocated )
    {
        assert( cstring );
        assert( compare.cstring );
        return ( strcmp( cstring, compare.cstring ) == 0 );
    }
    return false;
}


bool TiXmlString::operator < (const TiXmlString & compare) const
{
    if ( allocated && compare.allocated )
    {
        assert( cstring );
        assert( compare.cstring );
        return ( strcmp( cstring, compare.cstring ) > 0 );
    }
    return false;
}


bool TiXmlString::operator > (const TiXmlString & compare) const
{
    if ( allocated && compare.allocated )
    {
        assert( cstring );
        assert( compare.cstring );
        return ( strcmp( cstring, compare.cstring ) < 0 );
    }
    return false;
}
}