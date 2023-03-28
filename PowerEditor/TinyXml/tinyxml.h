#ifndef TINYXML_INCLUDED
#define TINYXML_INCLUDED

#define TIXML_ISTREAM   std::basic_istream<char>
#define TIXML_STRING    TiXmlString
#define TIXML_OSTREAM   TiXmlOutStream

#include <assert.h>
#include <tchar.h>
//#include <string>
#include <PowerEditor/MISC/Common/Common.h>
#include <PowerEditor/MISC/Common/FileInterface.h>

class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;

/*
   TiXmlString is an emulation of the String template.
   Its purpose is to allow compiling TinyXML on compilers with no or poor STL support.
   Only the member functions relevant to the TinyXML project have been implemented.
   The buffer allocation is made by a simplistic power of 2 like mechanism : if we increase
   a String and there's no more room, we allocate a buffer twice as big as we need.
*/
class TiXmlString
{
  public :
    // TiXmlString constructor, based on a String
    TiXmlString (const char* instring);

    // TiXmlString empty constructor
    TiXmlString ();

    // TiXmlString copy constructor
    TiXmlString (const TiXmlString& copy);

    // TiXmlString destructor
    ~ TiXmlString ();

    // Convert a TiXmlString into a classical char*
    const char* c_str () const;

    // Return the length of a TiXmlString
    unsigned length () const;

    // TiXmlString = operator
    void operator = (const char* content);

    // = operator
    void operator = (const TiXmlString & copy);

    // += operator. Maps to append
    TiXmlString& operator += (const char* suffix);

    // += operator. Maps to append
    TiXmlString& operator += (char single);

    // += operator. Maps to append
    TiXmlString& operator += (TiXmlString & suffix);

    bool operator == (const TiXmlString & compare) const;
    bool operator < (const TiXmlString & compare) const;
    bool operator > (const TiXmlString & compare) const;

    // Checks if a TiXmlString is empty
    bool empty () const;

    // Checks if a TiXmlString contains only whitespace (same rules as isspace)
    // Not actually used in tinyxml. Conflicts with a C macro, TEXT("isblank"),
    // which is a problem. Commenting out. -lee
//    bool isblank () const;

    // single char extraction
    const char& at (unsigned index) const;

    // find a char in a String. Return TiXmlString::notfound if not found
    unsigned find (char lookup) const;

    // find a char in a String from an offset. Return TiXmlString::notfound if not found
    unsigned find (char tofind, unsigned offset) const;

    /*  Function to reserve a big amount of data when we know we'll need it. Be aware that this
        function clears the content of the TiXmlString if any exists.
    */
    void reserve (unsigned size);

    // [] operator
    char& operator [] (unsigned index) const;

    // Error value for find primitive
    enum {  notfound = 0xffffffff,
            npos = notfound };

    void append (const char*str, int len );

  protected :

    // The base String
    char* cstring;
    // Number of chars allocated
    unsigned allocated;
    // Current String size
    unsigned current_length;

    // New size computation. It is simplistic right now : it returns twice the amount
    // we need
    unsigned assign_new_size (unsigned minimum_to_allocate);

    // Internal function that clears the content of a TiXmlString
    void empty_it ();

    void append (const char*suffix );

    // append function for another TiXmlString
    void append (const TiXmlString & suffix);

    // append for a single char. This could be improved a lot if needed
    void append (char single);

} ;

/*
   TiXmlOutStream is an emulation of std::ostream. It is based on TiXmlString.
   Only the operators that we need for TinyXML have been developped.
*/
class TiXmlOutStream : public TiXmlString
{
public :
    TiXmlOutStream () : TiXmlString () {}

    // TiXmlOutStream << operator. Maps to TiXmlString::append
    TiXmlOutStream & operator << (const char* in)
    {
        append (in);
        return (* this);
    }

    // TiXmlOutStream << operator. Maps to TiXmlString::append
    TiXmlOutStream & operator << (const TiXmlString & in)
    {
        append (in . c_str ());
        return (* this);
    }
} ;


/*  Internal structure for tracking location of items
    in the XML file.
*/
struct TiXmlCursor
{
    TiXmlCursor()       { Clear(); }
    void Clear()        { row = col = -1; }

    int row;    // 0 based.
    int col;    // 0 based.
};


// Only used by Attribute::Query functions
enum
{
    TIXML_SUCCESS,
    TIXML_NO_ATTRIBUTE,
    TIXML_WRONG_TYPE
};

/** TiXmlBase is a base class for every class in TinyXml.
    It does little except to establish that TinyXml classes
    can be printed and provide some utility functions.

    In XML, the document and elements can contain
    other elements and other types of nodes.

    @verbatim
    A Document can contain: Element (container or leaf)
                            Comment (leaf)
                            Unknown (leaf)
                            Declaration( leaf )

    An Element can contain: Element (container or leaf)
                            Text    (leaf)
                            Attributes (not on tree)
                            Comment (leaf)
                            Unknown (leaf)

    A Decleration contains: Attributes (not on tree)
    @endverbatim
*/
class TiXmlBase
{
    friend class TiXmlNode;
    friend class TiXmlElement;
    friend class TiXmlDocument;

public:
    TiXmlBase()                             {}
    virtual ~TiXmlBase()                    {}

    /** All TinyXml classes can print themselves to a filestream.
        This is a formatted print, and will insert tabs and newlines.

        (For an unformatted stream, use the << operator.)
    */
    virtual void Print( String& outputStream, int depth ) const = 0;

    /** The world does not agree on whether white space should be kept or
        not. In order to make everyone happy, these global, static functions
        are provided to set whether or not TinyXml will condense all white space
        into a single space or not. The default is to condense. Note changing this
        values is not thread safe.
    */
    static void SetCondenseWhiteSpace( bool condense )      { condenseWhiteSpace = condense; }

    /// Return the current white space setting.
    static bool IsWhiteSpaceCondensed()                     { return condenseWhiteSpace; }

    /** Return the position, in the original source file, of this node or attribute.
        The row and column are 1-based. (That is the first row and first column is
        1,1). If the returns values are 0 or less, then the parser does not have
        a row and column value.

        Generally, the row and column value will be set when the TiXmlDocument::Load(),
        TiXmlDocument::LoadFile(), or any TiXmlNode::Parse() is called. It will NOT be set
        when the DOM was created from operator>>.

        The values reflect the initial load. Once the DOM is modified programmatically
        (by adding or changing nodes and attributes) the new values will NOT update to
        reflect changes in the document.

        There is a minor performance cost to computing the row and column. Computation
        can be disabled if TiXmlDocument::SetTabSize() is called with 0 as the value.

        @sa TiXmlDocument::SetTabSize()
    */
    int Row() const         { return location.row + 1; }
    int Column() const      { return location.col + 1; }    ///< See Row()

protected:
    // See STL_STRING_BUG
    // Utility class to overcome a bug.
    class StringToBuffer
    {
      public:
        StringToBuffer( const TIXML_STRING& str );
        ~StringToBuffer();
        char* buffer;
    };

    static const char*  SkipWhiteSpace( const char* );
    inline static bool  IsWhiteSpace( int c )       { return ( _istspace( static_cast<char>(c) ) || c == '\n' || c == '\r' ); }

    virtual void StreamOut (TIXML_OSTREAM *) const = 0;

    #ifdef TIXML_USE_STL
        static bool StreamWhiteSpace( TIXML_ISTREAM * in, TIXML_STRING * tag );
        static bool StreamTo( TIXML_ISTREAM * in, int character, TIXML_STRING * tag );
    #endif

    /*  Reads an XML name into the String provided. Returns
        a pointer just past the last character of the name,
        or 0 if the function has an error.
    */
    static const char* ReadName( const char* p, TIXML_STRING* name );

    /*  Reads text. Returns a pointer past the given end tag.
        Wickedly complex options, but it keeps the (sensitive) code in one place.
    */
    static const char* ReadText(    const char* in,             // where to start
                                    TIXML_STRING* text,         // the String read
                                    bool ignoreWhiteSpace,      // whether to keep the white space
                                    const char* endTag,         // what ends this text
                                    bool ignoreCase );          // whether to ignore case in the end tag

    virtual const char* Parse( const char* p, TiXmlParsingData* data ) = 0;

    // If an entity has been found, transform it into a character.
    static const char* GetEntity( const char* in, char* value );

    // Get a character, while interpreting entities.
    inline static const char* GetChar( const char* p, char* _value )
    {
        assert( p );
        if ( *p == '&' )
        {
            return GetEntity( p, _value );
        }
        else
        {
            *_value = *p;
            return p+1;
        }
    }

    // Puts a String to a stream, expanding entities as it goes.
    // Note this should not contian the '<', '>', etc, or they will be transformed into entities!
    static void PutString( const TIXML_STRING& str, TIXML_OSTREAM* out );

    static void PutString( const TIXML_STRING& str, TIXML_STRING* out );

    // Return true if the next characters in the stream are any of the endTag sequences.
    static bool StringEqual(    const char* p,
                                const char* endTag,
                                bool ignoreCase );


    enum
    {
        TIXML_NO_ERROR = 0,
        TIXML_ERROR,
        TIXML_ERROR_OPENING_FILE,
        TIXML_ERROR_OUT_OF_MEMORY,
        TIXML_ERROR_PARSING_ELEMENT,
        TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
        TIXML_ERROR_READING_ELEMENT_VALUE,
        TIXML_ERROR_READING_ATTRIBUTES,
        TIXML_ERROR_PARSING_EMPTY,
        TIXML_ERROR_READING_END_TAG,
        TIXML_ERROR_PARSING_UNKNOWN,
        TIXML_ERROR_PARSING_COMMENT,
        TIXML_ERROR_PARSING_DECLARATION,
        TIXML_ERROR_DOCUMENT_EMPTY,

        TIXML_ERROR_STRING_COUNT
    };
    static const char* errorString[ TIXML_ERROR_STRING_COUNT ];

    TiXmlCursor location;

private:
    struct Entity
    {
        const char*     str;
        unsigned int    strLength;
        char            chr;
    };
    enum
    {
        NUM_ENTITY = 5,
        MAX_ENTITY_LENGTH = 6

    };
    static Entity entity[ NUM_ENTITY ];
    static bool condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
    (Except for attributes).
    Nodes have siblings, a parent, and children. A node can be
    in a document, or stand on its own. The type of a TiXmlNode
    can be queried, and it can be cast to its more defined type.
*/
class TiXmlNode : public TiXmlBase
{
    friend class TiXmlDocument;
    friend class TiXmlElement;

public:
    #ifdef TIXML_USE_STL

        /** An input stream operator, for every class. Tolerant of newlines and
            formatting, but doesn't expect them.
        */
        friend std::basic_istream<char>& operator >> (std::basic_istream<char>& in, TiXmlNode& base);

        /** An output stream operator, for every class. Note that this outputs
            without any newlines or formatting, as opposed to Print(), which
            includes tabs and new lines.

            The operator<< and operator>> are not completely symmetric. Writing
            a node to a stream is very well defined. You'll get a nice stream
            of output, without any extra whitespace or newlines.

            But reading is not as well defined. (As it always is.) If you create
            a TiXmlElement (for example) and read that from an input stream,
            the text needs to define an element or junk will result. This is
            true of all input streams, but it's worth keeping in mind.

            A TiXmlDocument will read nodes until it reads a root element, and
            all the children of that root element.
        */
        friend std::basic_ostream<char>& operator<< (std::basic_ostream<char>& out, const TiXmlNode& base);

        /// Appends the XML node or attribute to a String.
        friend String& operator<< (String& out, const TiXmlNode& base );

    #else
        // Used internally, not part of the public API.
        friend TIXML_OSTREAM& operator<< (TIXML_OSTREAM& out, const TiXmlNode& base);
    #endif

    /** The types of XML nodes supported by TinyXml. (All the
            unsupported types are picked up by UNKNOWN.)
    */
    enum NodeType
    {
        DOCUMENT,
        ELEMENT,
        COMMENT,
        UNKNOWN,
        TEXT,
        DECLARATION,
        TYPECOUNT
    };

    virtual ~TiXmlNode();

    /** The meaning of 'value' changes for the specific type of
        TiXmlNode.
        @verbatim
        Document:   filename of the xml file
        Element:    name of the element
        Comment:    the comment text
        Unknown:    the tag contents
        Text:       the text String
        @endverbatim

        The subclasses will wrap this function.
    */
    const char* Value() const { return value.c_str (); }

    /** Changes the value of the node. Defined as:
        @verbatim
        Document:   filename of the xml file
        Element:    name of the element
        Comment:    the comment text
        Unknown:    the tag contents
        Text:       the text String
        @endverbatim
    */
    void SetValue(const char* _value) { value = _value;}

    #ifdef TIXML_USE_STL
    /// STL String form.
    void SetValue( const String& _value )
    {
        StringToBuffer buf( _value );
        SetValue( buf.buffer ? buf.buffer : TEXT("") );
    }
    #endif

    /// Delete all the children of this node. Does not affect 'this'.
    void Clear();

    /// One step up the DOM.
    TiXmlNode* Parent() const                   { return parent; }

    TiXmlNode* FirstChild() const   { return firstChild; }      ///< The first child of this node. Will be null if there are no children.
    TiXmlNode* FirstChild( const char* value ) const;           ///< The first child of this node with the matching 'value'. Will be null if none found.

    TiXmlNode* LastChild() const    { return lastChild; }       /// The last child of this node. Will be null if there are no children.
    TiXmlNode* LastChild( const char* value ) const;            /// The last child of this node matching 'value'. Will be null if there are no children.

    #ifdef TIXML_USE_STL
    TiXmlNode* FirstChild( const String& _value ) const {   return FirstChild (_value.c_str ());    }   ///< STL std::String form.
    TiXmlNode* LastChild( const String& _value ) const      {   return LastChild (_value.c_str ()); }   ///< STL std::String form.
    #endif

    /** An alternate way to walk the children of a node.
        One way to iterate over nodes is:
        @verbatim
            for( child = parent->FirstChild(); child; child = child->NextSibling() )
        @endverbatim

        IterateChildren does the same thing with the syntax:
        @verbatim
            child = 0;
            while( child = parent->IterateChildren( child ) )
        @endverbatim

        IterateChildren takes the previous child as input and finds
        the next one. If the previous child is null, it returns the
        first. IterateChildren will return null when done.
    */
    TiXmlNode* IterateChildren( TiXmlNode* previous ) const;

    /// This flavor of IterateChildren searches for children with a particular 'value'
    TiXmlNode* IterateChildren( const char* value, TiXmlNode* previous ) const;

    #ifdef TIXML_USE_STL
    TiXmlNode* IterateChildren( const String& _value, TiXmlNode* previous ) const   {   return IterateChildren (_value.c_str (), previous); }   ///< STL std::String form.
    #endif

    /** Add a new node related to this. Adds a child past the LastChild.
        Returns a pointer to the new object or NULL if an error occured.
    */
    TiXmlNode* InsertEndChild( const TiXmlNode& addThis );


    /** Add a new node related to this. Adds a child past the LastChild.

        NOTE: the node to be added is passed by pointer, and will be
        henceforth owned (and deleted) by tinyXml. This method is efficient
        and avoids an extra copy, but should be used with care as it
        uses a different memory model than the other insert functions.

        @sa InsertEndChild
    */
    TiXmlNode* LinkEndChild( TiXmlNode* addThis );

    /** Add a new node related to this. Adds a child before the specified child.
        Returns a pointer to the new object or NULL if an error occured.
    */
    TiXmlNode* InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis );

    /** Add a new node related to this. Adds a child after the specified child.
        Returns a pointer to the new object or NULL if an error occured.
    */
    TiXmlNode* InsertAfterChild(  TiXmlNode* afterThis, const TiXmlNode& addThis );

    /** Replace a child of this node.
        Returns a pointer to the new object or NULL if an error occured.
    */
    TiXmlNode* ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis );

    /// Delete a child of this node.
    bool RemoveChild( TiXmlNode* removeThis );

    /// Navigate to a sibling node.
    TiXmlNode* PreviousSibling() const          { return prev; }

    /// Navigate to a sibling node.
    TiXmlNode* PreviousSibling( const char* ) const;

    #ifdef TIXML_USE_STL
    TiXmlNode* PreviousSibling( const String& _value ) const    {   return PreviousSibling (_value.c_str ());   }   ///< STL std::String form.
    TiXmlNode* NextSibling( const String& _value) const     {   return NextSibling (_value.c_str ());   }   ///< STL std::String form.
    #endif

    /// Navigate to a sibling node.
    TiXmlNode* NextSibling() const              { return next; }

    /// Navigate to a sibling node with the given 'value'.
    TiXmlNode* NextSibling( const char* ) const;

    /** Convenience function to get through elements.
        Calls NextSibling and ToElement. Will skip all non-Element
        nodes. Returns 0 if there is not another element.
    */
    TiXmlElement* NextSiblingElement() const;

    /** Convenience function to get through elements.
        Calls NextSibling and ToElement. Will skip all non-Element
        nodes. Returns 0 if there is not another element.
    */
    TiXmlElement* NextSiblingElement( const char* ) const;

    #ifdef TIXML_USE_STL
    TiXmlElement* NextSiblingElement( const String& _value) const   {   return NextSiblingElement (_value.c_str ());    }   ///< STL std::String form.
    #endif

    /// Convenience function to get through elements.
    TiXmlElement* FirstChildElement()   const;

    /// Convenience function to get through elements.
    TiXmlElement* FirstChildElement( const char* value ) const;

    #ifdef TIXML_USE_STL
    TiXmlElement* FirstChildElement( const String& _value ) const   {   return FirstChildElement (_value.c_str ()); }   ///< STL std::String form.
    #endif

    /** Query the type (as an enumerated value, above) of this node.
        The possible types are: DOCUMENT, ELEMENT, COMMENT,
                                UNKNOWN, TEXT, and DECLARATION.
    */
    virtual int Type() const    { return type; }

    /** Return a pointer to the Document this node lives in.
        Returns null if not in a document.
    */
    TiXmlDocument* GetDocument() const;

    /// Returns true if this node has no children.
    bool NoChildren() const                     { return !firstChild; }

    TiXmlDocument* ToDocument() const       { return ( this && type == DOCUMENT ) ? (TiXmlDocument*) this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
    TiXmlElement*  ToElement() const        { return ( this && type == ELEMENT  ) ? (TiXmlElement*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
    TiXmlComment*  ToComment() const        { return ( this && type == COMMENT  ) ? (TiXmlComment*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
    TiXmlUnknown*  ToUnknown() const        { return ( this && type == UNKNOWN  ) ? (TiXmlUnknown*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
    TiXmlText*     ToText()    const        { return ( this && type == TEXT     ) ? (TiXmlText*)     this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
    TiXmlDeclaration* ToDeclaration() const { return ( this && type == DECLARATION ) ? (TiXmlDeclaration*) this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.

    virtual TiXmlNode* Clone() const = 0;

    void  SetUserData( void* user )         { userData = user; }
    void* GetUserData()                     { return userData; }

protected:
    TiXmlNode( NodeType type );

    #ifdef TIXML_USE_STL
        // The real work of the input operator.
        virtual void StreamIn( TIXML_ISTREAM* in, TIXML_STRING* tag ) = 0;
    #endif

    // Figure out what is at *p, and parse it. Returns null if it is not an xml node.
    TiXmlNode* Identify( const char* start );
    void CopyToClone( TiXmlNode* target ) const { target->SetValue (value.c_str() );
                                                  target->userData = userData; }

    // Internal Value function returning a TIXML_STRING
    TIXML_STRING SValue() const { return value ; }

    TiXmlNode*      parent;
    NodeType        type;

    TiXmlNode*      firstChild;
    TiXmlNode*      lastChild;

    TIXML_STRING    value;

    TiXmlNode*      prev;
    TiXmlNode*      next;
    void*           userData;
};


/** An attribute is a name-value pair. Elements have an arbitrary
    number of attributes, each with a unique name.

    @note The attributes are not TiXmlNodes, since they are not
          part of the tinyXML document object model. There are other
          suggested ways to look at this problem.
*/
class TiXmlAttribute : public TiXmlBase
{
    friend class TiXmlAttributeSet;

public:
    /// Construct an empty attribute.
    TiXmlAttribute()
    {
        document = 0;
        prev = next = 0;
    }

    #ifdef TIXML_USE_STL
    /// String constructor.
    TiXmlAttribute( const String& _name, const String& _value )
    {
        name = _name;
        value = _value;
        document = 0;
        prev = next = 0;
    }
    #endif

    /// Construct an attribute with a name and value.
    TiXmlAttribute( const char* _name, const char* _value )
    {
        name = _name;
        value = _value;
        document = 0;
        prev = next = 0;
    }

    const char*     Name()  const       { return name.c_str (); }       ///< Return the name of this attribute.
    const char*     Value() const       { return value.c_str (); }      ///< Return the value of this attribute.
    const int       IntValue() const;                                   ///< Return the value of this attribute, converted to an integer.
    const double    DoubleValue() const;                                ///< Return the value of this attribute, converted to a double.

    /** QueryIntValue examines the value String. It is an alternative to the
        IntValue() method with richer error checking.
        If the value is an integer, it is stored in 'value' and
        the call returns TIXML_SUCCESS. If it is not
        an integer, it returns TIXML_WRONG_TYPE.

        A specialized but useful call. Note that for success it returns 0,
        which is the opposite of almost all other TinyXml calls.
    */
    int QueryIntValue( int* value ) const;
    /// QueryDoubleValue examines the value String. See QueryIntValue().
    int QueryDoubleValue( double* value ) const;

    void SetName( const char* _name )   { name = _name; }               ///< Set the name of this attribute.
    void SetValue( const char* _value ) { value = _value; }             ///< Set the value.

    void SetIntValue( int value );                                      ///< Set the value from an integer.
    void SetDoubleValue( double value );                                ///< Set the value from a double.

    #ifdef TIXML_USE_STL
    /// STL String form.
    void SetName( const String& _name )
    {
        StringToBuffer buf( _name );
        SetName ( buf.buffer ? buf.buffer : TEXT("error") );
    }
    /// STL String form.
    void SetValue( const String& _value )
    {
        StringToBuffer buf( _value );
        SetValue( buf.buffer ? buf.buffer : TEXT("error") );
    }
    #endif

    /// Get the next sibling attribute in the DOM. Returns null at end.
    TiXmlAttribute* Next() const;
    /// Get the previous sibling attribute in the DOM. Returns null at beginning.
    TiXmlAttribute* Previous() const;

    bool operator==( const TiXmlAttribute& rhs ) const { return rhs.name == name; }
    bool operator<( const TiXmlAttribute& rhs )  const { return name < rhs.name; }
    bool operator>( const TiXmlAttribute& rhs )  const { return name > rhs.name; }

    /*  [internal use]
        Attribtue parsing starts: first letter of the name
                         returns: the next char after the value end quote
    */
    virtual const char* Parse( const char* p, TiXmlParsingData* data );

    // [internal use]
    virtual void Print( String& outputStream, int depth ) const;

    virtual void StreamOut( TIXML_OSTREAM * out ) const;
    // [internal use]
    // Set the document pointer so the attribute can report errors.
    void SetDocument( TiXmlDocument* doc )  { document = doc; }

private:
    TiXmlDocument*  document;   // A pointer back to a document, for error reporting.
    TIXML_STRING name;
    TIXML_STRING value;
    TiXmlAttribute* prev;
    TiXmlAttribute* next;
};


/*  A class used to manage a group of attributes.
    It is only used internally, both by the ELEMENT and the DECLARATION.

    The set can be changed transparent to the Element and Declaration
    classes that use it, but NOT transparent to the Attribute
    which has to implement a next() and previous() method. Which makes
    it a bit problematic and prevents the use of STL.

    This version is implemented with circular lists because:
        - I like circular lists
        - it demonstrates some independence from the (typical) doubly linked list.
*/
class TiXmlAttributeSet
{
public:
    TiXmlAttributeSet();
    ~TiXmlAttributeSet();

    void Add( TiXmlAttribute* attribute );
    void Remove( TiXmlAttribute* attribute );

    TiXmlAttribute* First() const   { return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
    TiXmlAttribute* Last()  const   { return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }
    TiXmlAttribute* Find( const char* name ) const;

private:
    TiXmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
    and can contain other elements, text, comments, and unknowns.
    Elements also contain an arbitrary number of attributes.
*/
class TiXmlElement : public TiXmlNode
{
public:
    /// Construct an element.
    TiXmlElement (const char* in_value);

    #ifdef TIXML_USE_STL
    /// String constructor.
    TiXmlElement( const String& _value ) :  TiXmlNode( TiXmlNode::ELEMENT )
    {
        firstChild = lastChild = 0;
        value = _value;
    }
    #endif

    virtual ~TiXmlElement();

    /** Given an attribute name, Attribute() returns the value
        for the attribute of that name, or null if none exists.
    */
    const char* Attribute( const char* name ) const;

    /** Given an attribute name, Attribute() returns the value
        for the attribute of that name, or null if none exists.
        If the attribute exists and can be converted to an integer,
        the integer value will be put in the return 'i', if 'i'
        is non-null.
    */
    const char* Attribute( const char* name, int* i ) const;

    /** Given an attribute name, Attribute() returns the value
        for the attribute of that name, or null if none exists.
        If the attribute exists and can be converted to an double,
        the double value will be put in the return 'd', if 'd'
        is non-null.
    */
    const char* Attribute( const char* name, double* d ) const;

    /** QueryIntAttribute examines the attribute - it is an alternative to the
        Attribute() method with richer error checking.
        If the attribute is an integer, it is stored in 'value' and
        the call returns TIXML_SUCCESS. If it is not
        an integer, it returns TIXML_WRONG_TYPE. If the attribute
        does not exist, then TIXML_NO_ATTRIBUTE is returned.
    */
    int QueryIntAttribute( const char* name, int* value ) const;
    /// QueryDoubleAttribute examines the attribute - see QueryIntAttribute().
    int QueryDoubleAttribute( const char* name, double* value ) const;

    /** Sets an attribute of name to a given value. The attribute
        will be created if it does not exist, or changed if it does.
    */
    void SetAttribute( const char* name, const char* value );

    #ifdef TIXML_USE_STL
    const char* Attribute( const String& name ) const               { return Attribute( name.c_str() ); }
    const char* Attribute( const String& name, int* i ) const       { return Attribute( name.c_str(), i ); }

    /// STL String form.
    void SetAttribute( const String& name, const String& _value )
    {
        StringToBuffer n( name );
        StringToBuffer v( _value );
        if ( n.buffer && v.buffer )
            SetAttribute (n.buffer, v.buffer );
    }
    ///< STL String form.
    void SetAttribute( const String& name, int _value )
    {
        StringToBuffer n( name );
        if ( n.buffer )
            SetAttribute (n.buffer, _value);
    }
    #endif

    /** Sets an attribute of name to a given value. The attribute
        will be created if it does not exist, or changed if it does.
    */
    void SetAttribute( const char* name, int value );

    /** Deletes an attribute with the given name.
    */
    void RemoveAttribute( const char* name );
    #ifdef TIXML_USE_STL
    void RemoveAttribute( const String& name )  {   RemoveAttribute (name.c_str ());    }   ///< STL String form.
    #endif

    TiXmlAttribute* FirstAttribute() const  { return attributeSet.First(); }        ///< Access the first attribute in this element.
    TiXmlAttribute* LastAttribute() const   { return attributeSet.Last(); }     ///< Access the last attribute in this element.

    // [internal use] Creates a new Element and returs it.
    virtual TiXmlNode* Clone() const;
    // [internal use]

    virtual void Print( String& outputStream, int depth ) const;

protected:

    // Used to be public [internal use]
    #ifdef TIXML_USE_STL
        virtual void StreamIn( TIXML_ISTREAM * in, TIXML_STRING * tag );
    #endif
    virtual void StreamOut( TIXML_OSTREAM * out ) const;

    /*  [internal use]
        Attribtue parsing starts: next char past '<'
                         returns: next char past '>'
    */
    virtual const char* Parse( const char* p, TiXmlParsingData* data );

    /*  [internal use]
        Reads the "value" of the element -- another element, or text.
        This should terminate with the current end tag.
    */
    const char* ReadValue( const char* in, TiXmlParsingData* prevData );

private:
    TiXmlAttributeSet attributeSet;
};


/** An XML comment.
*/
class TiXmlComment : public TiXmlNode
{
public:
    /// Constructs an empty comment.
    TiXmlComment() : TiXmlNode( TiXmlNode::COMMENT ) {}
    virtual ~TiXmlComment() {}

    // [internal use] Creates a new Element and returs it.
    virtual TiXmlNode* Clone() const;
    // [internal use]
    virtual void Print( String& outputStream, int depth ) const;
protected:
    // used to be public
    #ifdef TIXML_USE_STL
        virtual void StreamIn( TIXML_ISTREAM * in, TIXML_STRING * tag );
    #endif
    virtual void StreamOut( TIXML_OSTREAM * out ) const;
    /*  [internal use]
        Attribtue parsing starts: at the ! of the !--
                         returns: next char past '>'
    */
    virtual const char* Parse( const char* p, TiXmlParsingData* data );
};


/** XML text. Contained in an element.
*/
class TiXmlText : public TiXmlNode
{
    friend class TiXmlElement;
public:
    /// Constructor.
    TiXmlText (const char* initValue) : TiXmlNode (TiXmlNode::TEXT)
    {
        SetValue( initValue );
    }
    virtual ~TiXmlText() {}

    #ifdef TIXML_USE_STL
    /// Constructor.
    TiXmlText( const String& initValue ) : TiXmlNode (TiXmlNode::TEXT)
    {
        SetValue( initValue );
    }
    #endif

    // [internal use]
    virtual void Print( String& outputStream, int depth ) const;

protected :
    // [internal use] Creates a new Element and returns it.
    virtual TiXmlNode* Clone() const;
    virtual void StreamOut ( TIXML_OSTREAM * out ) const;
    // [internal use]
    bool Blank() const; // returns true if all white space and new lines
    /*  [internal use]
            Attribtue parsing starts: First char of the text
                             returns: next char past '>'
    */
    virtual const char* Parse( const char* p, TiXmlParsingData* data );
    // [internal use]
    #ifdef TIXML_USE_STL
        virtual void StreamIn( TIXML_ISTREAM * in, TIXML_STRING * tag );
    #endif
};


/** In correct XML the declaration is the first entry in the file.
    @verbatim
        <?xml version="1.0" standalone="yes"?>
    @endverbatim

    TinyXml will happily read or write files without a declaration,
    however. There are 3 possible attributes to the declaration:
    version, encoding, and standalone.

    Note: In this version of the code, the attributes are
    handled as special cases, not generic attributes, simply
    because there can only be at most 3 and they are always the same.
*/
class TiXmlDeclaration : public TiXmlNode
{
public:
    /// Construct an empty declaration.
    TiXmlDeclaration()   : TiXmlNode( TiXmlNode::DECLARATION ) {}

#ifdef TIXML_USE_STL
    /// Constructor.
    TiXmlDeclaration(   const String& _version,
                        const String& _encoding,
                        const String& _standalone )
            : TiXmlNode( TiXmlNode::DECLARATION )
    {
        version = _version;
        encoding = _encoding;
        standalone = _standalone;
    }
#endif

    /// Construct.
    TiXmlDeclaration(   const char* _version,
                        const char* _encoding,
                        const char* _standalone );

    virtual ~TiXmlDeclaration() {}

    /// Version. Will return empty if none was found.
    const char* Version() const     { return version.c_str (); }
    /// Encoding. Will return empty if none was found.
    const char* Encoding() const        { return encoding.c_str (); }
    /// Is this a standalone document?
    const char* Standalone() const      { return standalone.c_str (); }

    // [internal use] Creates a new Element and returs it.
    virtual TiXmlNode* Clone() const;
    // [internal use]
    virtual void Print( String& outputStream, int depth ) const;

protected:
    // used to be public
    #ifdef TIXML_USE_STL
        virtual void StreamIn( TIXML_ISTREAM * in, TIXML_STRING * tag );
    #endif
    virtual void StreamOut ( TIXML_OSTREAM * out) const;
    //  [internal use]
    //  Attribtue parsing starts: next char past '<'
    //                   returns: next char past '>'

    virtual const char* Parse( const char* p, TiXmlParsingData* data );

private:
    TIXML_STRING version;
    TIXML_STRING encoding;
    TIXML_STRING standalone;
};


/** Any tag that tinyXml doesn't recognize is saved as an
    unknown. It is a tag of text, but should not be modified.
    It will be written back to the XML, unchanged, when the file
    is saved.
*/
class TiXmlUnknown : public TiXmlNode
{
public:
    TiXmlUnknown() : TiXmlNode( TiXmlNode::UNKNOWN ) {}
    virtual ~TiXmlUnknown() {}

    // [internal use]
    virtual TiXmlNode* Clone() const;
    // [internal use]
    virtual void Print( String& outputStream, int depth ) const;
protected:
    #ifdef TIXML_USE_STL
        virtual void StreamIn( TIXML_ISTREAM * in, TIXML_STRING * tag );
    #endif
    virtual void StreamOut ( TIXML_OSTREAM * out ) const;
    /*  [internal use]
        Attribute parsing starts: First char of the text
                         returns: next char past '>'
    */
    virtual const char* Parse( const char* p, TiXmlParsingData* data );
};


/** Always the top level node. A document binds together all the
    XML pieces. It can be saved, loaded, and printed to the screen.
    The 'value' of a document node is the xml file name.
*/
class TiXmlDocument : public TiXmlNode
{
public:
    /// Create an empty document, that has no name.
    TiXmlDocument();
    /// Create a document with a name. The name of the document is also the filename of the xml.
    TiXmlDocument( const char* documentName );

    #ifdef TIXML_USE_STL
    /// Constructor.
    TiXmlDocument( const String& documentName ) :
        TiXmlNode( TiXmlNode::DOCUMENT )
    {
        tabsize = 4;
        value = documentName;
        error = false;
    }
    #endif

    virtual ~TiXmlDocument() {}

    /** Load a file using the current document value.
        Returns true if successful. Will delete any existing
        document data before loading.
    */
    bool LoadFile();
    /// Save a file using the current document value. Returns true if successful.
    bool SaveFile() const;
    /// Load a file using the given filename. Returns true if successful.
    bool LoadFile( const char* filename );
    /// Save a file using the given filename. Returns true if successful.
    bool SaveFile( const char* filename ) const;

    #ifdef TIXML_USE_STL
    bool LoadFile( const String& filename )         ///< STL String version.
    {
        StringToBuffer f( filename );
        return ( f.buffer && LoadFile( f.buffer ));
    }
    bool SaveFile( const String& filename ) const       ///< STL String version.
    {
        StringToBuffer f( filename );
        return ( f.buffer && SaveFile( f.buffer ));
    }
    #endif

    /** Parse the given null terminated block of xml data.
    */
    virtual const char* Parse( const char* p, TiXmlParsingData* data = 0 );

    /** Get the root element -- the only top level element -- of the document.
        In well formed XML, there should only be one. TinyXml is tolerant of
        multiple elements at the document level.
    */
    TiXmlElement* RootElement() const       { return FirstChildElement(); }

    /** If an error occurs, Error will be set to true. Also,
        - The ErrorId() will contain the integer identifier of the error (not generally useful)
        - The ErrorDesc() method will return the name of the error. (very useful)
        - The ErrorRow() and ErrorCol() will return the location of the error (if known)
    */
    bool Error() const                      { return error; }

    /// Contains a textual (english) description of the error if one occurs.
    const char* ErrorDesc() const   { return errorDesc.c_str (); }

    /** Generally, you probably want the error String ( ErrorDesc() ). But if you
        prefer the ErrorId, this function will fetch it.
    */
    const int ErrorId() const               { return errorId; }

    /** Returns the location (if known) of the error. The first column is column 1,
        and the first row is row 1. A value of 0 means the row and column wasn't applicable
        (memory errors, for example, have no row/column) or the parser lost the error. (An
        error in the error reporting, in that case.)

        @sa SetTabSize, Row, Column
    */
    int ErrorRow()  { return errorLocation.row+1; }
    int ErrorCol()  { return errorLocation.col+1; } ///< The column where the error occured. See ErrorRow()

    /** By calling this method, with a tab size
        greater than 0, the row and column of each node and attribute is stored
        when the file is loaded. Very useful for tracking the DOM back in to
        the source file.

        The tab size is required for calculating the location of nodes. If not
        set, the default of 4 is used. The tabsize is set per document. Setting
        the tabsize to 0 disables row/column tracking.

        Note that row and column tracking is not supported when using operator>>.

        The tab size needs to be enabled before the parse or load. Correct usage:
        @verbatim
        TiXmlDocument doc;
        doc.SetTabSize( 8 );
        doc.Load( "myfile.xml" );
        @endverbatim

        @sa Row, Column
    */
    void SetTabSize( int _tabsize )     { tabsize = _tabsize; }

    int TabSize() const { return tabsize; }

    /** If you have handled the error, it can be reset with this call. The error
        state is automatically cleared if you Parse a new XML block.
    */
    void ClearError()                       {   error = false;
                                                errorId = 0;
                                                errorDesc = TEXT("");
                                                errorLocation.row = errorLocation.col = 0;
                                                //errorLocation.last = 0;
                                            }

    /** Dump the document to standard out. */
    void Print() const                      { /*Print(stdout, 0);*/ }

    // [internal use]
    virtual void Print( String& outputStream, int depth = 0 ) const;
    // [internal use]
    void SetError( int err, const char* errorLocation, TiXmlParsingData* prevData );

protected :
    virtual void StreamOut ( TIXML_OSTREAM * out) const;
    // [internal use]
    virtual TiXmlNode* Clone() const;
    #ifdef TIXML_USE_STL
        virtual void StreamIn( TIXML_ISTREAM * in, TIXML_STRING * tag );
    #endif

private:
    bool error;
    int  errorId;
    TIXML_STRING errorDesc;
    int tabsize;
    TiXmlCursor errorLocation;
};


/**
    A TiXmlHandle is a class that wraps a node pointer with null checks; this is
    an incredibly useful thing. Note that TiXmlHandle is not part of the TinyXml
    DOM structure. It is a separate utility class.

    Take an example:
    @verbatim
    <Document>
        <Element attributeA = TEXT("valueA")>
            <Child attributeB = TEXT("value1") />
            <Child attributeB = TEXT("value2") />
        </Element>
    <Document>
    @endverbatim

    Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very
    easy to write a *lot* of code that looks like:

    @verbatim
    TiXmlElement* root = document.FirstChildElement( "Document" );
    if ( root )
    {
        TiXmlElement* element = root->FirstChildElement( "Element" );
        if ( element )
        {
            TiXmlElement* child = element->FirstChildElement( "Child" );
            if ( child )
            {
                TiXmlElement* child2 = child->NextSiblingElement( "Child" );
                if ( child2 )
                {
                    // Finally do something useful.
    @endverbatim

    And that doesn't even cover "else" cases. TiXmlHandle addresses the verbosity
    of such code. A TiXmlHandle checks for null pointers so it is perfectly safe
    and correct to use:

    @verbatim
    TiXmlHandle docHandle( &document );
    TiXmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).Element();
    if ( child2 )
    {
        // do something useful
    @endverbatim

    Which is MUCH more concise and useful.

    It is also safe to copy handles - internally they are nothing more than node pointers.
    @verbatim
    TiXmlHandle handleCopy = handle;
    @endverbatim

    What they should not be used for is iteration:

    @verbatim
    int i=0;
    while ( true )
    {
        TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).Element();
        if ( !child )
            break;
        // do something
        ++i;
    }
    @endverbatim

    It seems reasonable, but it is in fact two embedded while loops. The Child method is
    a linear walk to find the element, so this code would iterate much more than it needs
    to. Instead, prefer:

    @verbatim
    TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).Element();

    for( child; child; child=child->NextSiblingElement() )
    {
        // do something
    }
    @endverbatim
*/
class TiXmlHandle
{
public:
    /// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
    TiXmlHandle( TiXmlNode* node )          { this->node = node; }
    /// Copy constructor
    TiXmlHandle( const TiXmlHandle& ref )   { this->node = ref.node; }

    /// Return a handle to the first child node.
    TiXmlHandle FirstChild() const;
    /// Return a handle to the first child node with the given name.
    TiXmlHandle FirstChild( const char* value ) const;
    /// Return a handle to the first child element.
    TiXmlHandle FirstChildElement() const;
    /// Return a handle to the first child element with the given name.
    TiXmlHandle FirstChildElement( const char* value ) const;

    /** Return a handle to the "index" child with the given name.
        The first child is 0, the second 1, etc.
    */
    TiXmlHandle Child( const char* value, int index ) const;
    /** Return a handle to the "index" child.
        The first child is 0, the second 1, etc.
    */
    TiXmlHandle Child( int index ) const;
    /** Return a handle to the "index" child element with the given name.
        The first child element is 0, the second 1, etc. Note that only TiXmlElements
        are indexed: other types are not counted.
    */
    TiXmlHandle ChildElement( const char* value, int index ) const;
    /** Return a handle to the "index" child element.
        The first child element is 0, the second 1, etc. Note that only TiXmlElements
        are indexed: other types are not counted.
    */
    TiXmlHandle ChildElement( int index ) const;

    #ifdef TIXML_USE_STL
    TiXmlHandle FirstChild( const String& _value ) const            { return FirstChild( _value.c_str() ); }
    TiXmlHandle FirstChildElement( const String& _value ) const     { return FirstChildElement( _value.c_str() ); }

    TiXmlHandle Child( const String& _value, int index ) const          { return Child( _value.c_str(), index ); }
    TiXmlHandle ChildElement( const String& _value, int index ) const   { return ChildElement( _value.c_str(), index ); }
    #endif

    /// Return the handle as a TiXmlNode. This may return null.
    TiXmlNode* Node() const         { return node; }
    /// Return the handle as a TiXmlElement. This may return null.
    TiXmlElement* Element() const   { return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
    /// Return the handle as a TiXmlText. This may return null.
    TiXmlText* Text() const         { return ( ( node && node->ToText() ) ? node->ToText() : 0 ); }

private:
    TiXmlNode* node;
};

#endif

