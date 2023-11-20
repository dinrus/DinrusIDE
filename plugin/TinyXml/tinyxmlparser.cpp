#include <sstream>
#include "tinyxml.h"

namespace UPP{
//#define DEBUG_PARSER

// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.
TiXmlBase::Entity TiXmlBase::entity[ NUM_ENTITY ] =
{
    { String("&amp;"),  5, '&' },
    { String("&lt;"),   4, '<' },
    { String("&gt;"),   4, '>' },
    { String("&quot;"), 6, '\"' },
    { String("&apos;"), 6, '\'' }
};


void TiXmlParsingData::Stamp( const char* now )
{
    assert( now );

    // Do nothing if the tabsize is 0.
    if ( tabsize < 1 )
    {
        return;
    }

    // Get the current row, column.
    int row = cursor.row;
    int col = cursor.col;
    const char* p = stamp;
    assert( p );

    while ( p < now )
    {
        // Code contributed by Fletcher Dunn: (modified by lee)
        switch (*p) {
            case 0:
                // We *should* never get here, but in case we do, don't
                // advance past the terminating null character, ever
                return;

            case '\r':
                // bump down to the next line
                ++row;
                col = 0;
                // Eat the character
                ++p;

                // Check for \r\n sequence, and treat this as a single character
                if (*p == '\n') {
                    ++p;
                }
                break;

            case '\n':
                // bump down to the next line
                ++row;
                col = 0;

                // Eat the character
                ++p;

                // Check for \n\r sequence, and treat this as a single
                // character.  (Yes, this bizarre thing does occur still
                // on some arcane platforms...)
                if (*p == '\r') {
                    ++p;
                }
                break;

            case '\t':
                // Eat the character
                ++p;

                // Skip to next tab stop
                col = (col / tabsize + 1) * tabsize;
                break;

            default:
                // Eat the character
                ++p;

                // Normal char - just advance one column
                ++col;
                break;
        }
    }
    cursor.row = row;
    cursor.col = col;
    assert( cursor.row >= -1 );
    assert( cursor.col >= -1 );
    stamp = p;
    assert( stamp );
}


const char* TiXmlBase::SkipWhiteSpace( const char* p )
{
    if ( !p || !*p )
    {
        return 0;
    }
    while ( p && *p )
    {
        if ( iswspace( *p ) || *p == '\n' || *p =='\r' )       // Still using old rules for white space.
            ++p;
        else
            break;
    }

    return p;
}

const char* TiXmlBase::ReadName( const char* p, TIXML_STRING * name )
{
    *name = String("");
    assert( p );

    // Names start with letters or underscores.
    // After that, they can be letters, underscores, numbers,
    // hyphens, or colons. (Colons are valid ony for namespaces,
    // but tinyxml can't tell namespaces from names.)
    if (    p && *p
         && ( iswalpha( *p ) || *p == '_' ) )
    {
        while(      p && *p
                &&  (       iswalnum( *p )
                         || *p == '_'
                         || *p == '-'
                         || *p == '.'
                         || *p == ':' ) )
        {
            (*name) += *p;
            ++p;
        }
        return p;
    }
    return 0;
}

const char* TiXmlBase::GetEntity( const char* p, char* value )
{
    // Presume an entity, and pull it out.
    int i;

    // Handle the &#x entities.
    if (strncmp( String("&#x"), p, 3 ) == 0)
    {
        const char* end = strchr(p+3, ';');
        if (end && end - p <= 3 + 4)
        {
            char* hexend;
            auto val = strtol(p + 3, &hexend, 16);
            if (hexend == end)
            {
                *value = static_cast<char>(val);
                return end + 1;
            }
        }
    }

    // Now try to match it.
    for (i=0; i<NUM_ENTITY; ++i)
    {
        if ( strncmp( entity[i].str, p, entity[i].strLength ) == 0 )
        {
            assert(static_cast<unsigned int>(strlen(entity[i].str)) == entity[i].strLength );
            *value = entity[i].chr;
            return (p + entity[i].strLength);
        }
    }

    // So it wasn't an entity, its unrecognized, or something like that.
    *value = *p;    // Don't put back the last one, since we return it!
    return p+1;
}


bool TiXmlBase::StringEqual( const char* p,
                             const char* tag,
                             bool ignoreCase )
{
    assert( p );
    if ( !p || !*p )
    {
        assert( 0 );
        return false;
    }

    if ( ToLower( *p ) == ToLower( *tag ) )
    {
        const char* q = p;

        if (ignoreCase)
        {
            while ( *q && *tag && *q == *tag )
            {
                ++q;
                ++tag;
            }

            if ( *tag == 0 )        // Have we found the end of the tag, and everything equal?
            {
                return true;
            }
        }
        else
        {
            while ( *q && *tag && ToLower( *q ) == ToLower( *tag ) )
            {
                ++q;
                ++tag;
            }

            if ( *tag == 0 )
            {
                return true;
            }
        }
    }
    return false;
}

const char* TiXmlBase::ReadText(    const char* p,
                                    TIXML_STRING * text,
                                    bool trimWhiteSpace,
                                    const char* endTag,
                                    bool caseInsensitive )
{
    *text = String("");
    if (    !trimWhiteSpace         // certain tags always keep whitespace
         || !condenseWhiteSpace )   // if true, whitespace is always kept
    {
        // Keep all the white space.
        while (    p && *p
                && !StringEqual( p, endTag, caseInsensitive )
              )
        {
            char c;
            p = GetChar( p, &c );
            (* text) += c;
        }
    }
    else
    {
        bool whitespace = false;

        // Remove leading white space:
        p = SkipWhiteSpace( p );
        while (    p && *p
                && !StringEqual( p, endTag, caseInsensitive ) )
        {
            if ( *p == '\r' || *p == '\n' )
            {
                whitespace = true;
                ++p;
            }
            else if ( iswspace( *p ) )
            {
                whitespace = true;
                ++p;
            }
            else
            {
                // If we've found whitespace, add it before the
                // new character. Any whitespace just becomes a space.
                if ( whitespace )
                {
               (* text) += ' ';
                    whitespace = false;
                }
                char c;
                p = GetChar( p, &c );
            (* text) += c;
            }
        }
    }
    return p + strlen( endTag );
}

const char* TiXmlDocument::Parse( const char* p, TiXmlParsingData* prevData )
{
    ClearError();

    // Parse away, at the document level. Since a document
    // contains nothing but other tags, most of what happens
    // here is skipping white space.
    if ( !p || !*p )
    {
        SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0 );
        return 0;
    }

    // Note that, for a document, this needs to come
    // before the while space skip, so that parsing
    // starts from the pointer we are given.
    location.Clear();
    if ( prevData )
    {
        location.row = prevData->cursor.row;
        location.col = prevData->cursor.col;
    }
    else
    {
        location.row = 0;
        location.col = 0;
    }
    TiXmlParsingData data( p, TabSize(), location.row, location.col );
    location = data.Cursor();

    p = SkipWhiteSpace( p );
    if ( !p )
    {
        SetError( TIXML_ERROR_DOCUMENT_EMPTY, 0, 0 );
        return 0;
    }

    while ( p && *p )
    {
        TiXmlNode* node = Identify( p );
        if ( node )
        {
            p = node->Parse( p, &data );
            LinkEndChild( node );
        }
        else
        {
            break;
        }
        p = SkipWhiteSpace( p );
    }

    // All is well.
    return p;
}

void TiXmlDocument::SetError( int err, const char* pError, TiXmlParsingData* data )
{
    // The first error in a chain is more accurate - don't set again!
    if ( error )
        return;

    assert( err > 0 && err < TIXML_ERROR_STRING_COUNT );
    error   = true;
    errorId = err;
    errorDesc = errorString[ errorId ];

    errorLocation.Clear();
    if ( pError && data )
    {
        //TiXmlParsingData data( pError, prevData );
        data->Stamp( pError );
        errorLocation = data->Cursor();
    }
}


TiXmlNode* TiXmlNode::Identify( const char* p )
{
    TiXmlNode* returnNode = 0;

    p = SkipWhiteSpace( p );
    if ( !p || !*p || *p != '<' )
    {
        return 0;
    }

    TiXmlDocument* doc = GetDocument();
    p = SkipWhiteSpace( p );

    if ( !p || !*p )
    {
        return 0;
    }

    // What is this thing?
    // - Elements start with a letter or underscore, but xml is reserved.
    // - Comments: <!--
    // - Decleration: <?xml
    // - Everthing else is unknown to tinyxml.
    //

    const char* xmlHeader = { "<?xml"};
    const char* commentHeader = { "<!--" };

    if ( StringEqual( p, xmlHeader, true ) )
    {
        #ifdef DEBUG_PARSER
            TIXML_LOG( "XML parsing Declaration\n" );
        #endif
        returnNode = new TiXmlDeclaration();
    }
    else if (    iswalpha( *(p+1) )
              || *(p+1) == '_' )
    {
        #ifdef DEBUG_PARSER
            TIXML_LOG( "XML parsing Element\n" );
        #endif
        returnNode = new TiXmlElement( "" );
    }
    else if ( StringEqual( p, commentHeader, false ) )
    {
        #ifdef DEBUG_PARSER
            TIXML_LOG( "XML parsing Comment\n" );
        #endif
        returnNode = new TiXmlComment();
    }
    else
    {
        #ifdef DEBUG_PARSER
            TIXML_LOG( "XML parsing Unknown\n" );
        #endif
        returnNode = new TiXmlUnknown();
    }

    if ( returnNode )
    {
        // Set the parent, so it can report errors
        returnNode->parent = this;
    }
    else
    {
        if ( doc )
            doc->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0 );
    }
    return returnNode;
}

const char* TiXmlElement::Parse( const char* p, TiXmlParsingData* data )
{
    p = SkipWhiteSpace( p );
    TiXmlDocument* document = GetDocument();

    if ( !p || !*p )
    {
        if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, 0, 0 );
        return 0;
    }

//  TiXmlParsingData data( p, prevData );
    if ( data )
    {
        data->Stamp( p );
        location = data->Cursor();
    }

    if ( *p != '<' )
    {
        if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, p, data );
        return 0;
    }

    p = SkipWhiteSpace( p+1 );

    // Read the name.
    const char* pErr = p;

    p = ReadName( p, &value );
    if ( !p || !*p )
    {
        if ( document ) document->SetError( TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data );
        return 0;
    }

    TIXML_STRING endTag ("</");
    endTag += value;
    endTag += ">";

    // Check for and read attributes. Also look for an empty
    // tag or an end tag.
    while ( p && *p )
    {
        pErr = p;
        p = SkipWhiteSpace( p );
        if ( !p || !*p )
        {
            if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, pErr, data );
            return 0;
        }
        if ( *p == '/' )
        {
            ++p;
            // Empty tag.
            if ( *p  != '>' )
            {
                if ( document ) document->SetError( TIXML_ERROR_PARSING_EMPTY, p, data );
                return 0;
            }
            return (p+1);
        }
        else if ( *p == '>' )
        {
            // Done with attributes (if there were any.)
            // Read the value -- which can include other
            // elements -- read the end tag, and return.
            ++p;
            p = ReadValue( p, data );       // Note this is an Element method, and will set the error if one happens.
            if ( !p || !*p )
                return 0;

            // We should find the end tag now
            if ( StringEqual( p, endTag.c_str(), false ) )
            {
                p += endTag.length();
                return p;
            }
            else
            {
                if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG, p, data );
                return 0;
            }
        }
        else
        {
            // Try to read an attribute:
            TiXmlAttribute* attrib = new TiXmlAttribute();
            if ( !attrib )
            {
                if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, pErr, data );
                return 0;
            }

            attrib->SetDocument( document );
            const char* pErr = p;
            p = attrib->Parse( p, data );

            if ( !p || !*p )
            {
                if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT, pErr, data );
                delete attrib;
                return 0;
            }

            // Handle the strange case of double attributes:
            TiXmlAttribute* node = attributeSet.Find( attrib->Name() );
            if ( node )
            {
                node->SetValue( attrib->Value() );
                delete attrib;
                return 0;
            }

            attributeSet.Add( attrib );
        }
    }
    return p;
}


const char* TiXmlElement::ReadValue( const char* p, TiXmlParsingData* data )
{
    TiXmlDocument* document = GetDocument();

    // Read in text and elements in any order.
    p = SkipWhiteSpace( p );
    while ( p && *p )
    {
        if ( *p != '<' )
        {
            // Take what we have, make a text element.
            TiXmlText* textNode = new TiXmlText( "" );

            if ( !textNode )
            {
                if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY, 0, 0 );
                    return 0;
            }

            p = textNode->Parse( p, data );

            if ( !textNode->Blank() )
                LinkEndChild( textNode );
            else
                delete textNode;
        }
        else
        {
            // We hit a '<'
            // Have we hit a new element or an end tag?
            if ( StringEqual( p, "</", false ) )
            {
                return p;
            }
            else
            {
                TiXmlNode* node = Identify( p );
                if ( node )
                {
                    p = node->Parse( p, data );
                    LinkEndChild( node );
                }
                else
                {
                    return 0;
                }
            }
        }
        p = SkipWhiteSpace( p );
    }

    if ( !p )
    {
        if ( document ) document->SetError( TIXML_ERROR_READING_ELEMENT_VALUE, 0, 0 );
    }
    return p;
}


const char* TiXmlUnknown::Parse( const char* p, TiXmlParsingData* data )
{
    TiXmlDocument* document = GetDocument();
    p = SkipWhiteSpace( p );

//  TiXmlParsingData data( p, prevData );
    if ( data )
    {
        data->Stamp( p );
        location = data->Cursor();
    }
    if ( !p || !*p || *p != '<' )
    {
        if ( document ) document->SetError( TIXML_ERROR_PARSING_UNKNOWN, p, data );
        return 0;
    }
    ++p;
    value = "";

    while ( p && *p && *p != '>' )
    {
        value += *p;
        ++p;
    }

    if ( !p )
    {
        if ( document ) document->SetError( TIXML_ERROR_PARSING_UNKNOWN, 0, 0 );
    }
    if ( *p == '>' )
        return p+1;
    return p;
}

const char* TiXmlComment::Parse( const char* p, TiXmlParsingData* data )
{
    TiXmlDocument* document = GetDocument();
    value = "";

    p = SkipWhiteSpace( p );

//  TiXmlParsingData data( p, prevData );
    if ( data )
    {
        data->Stamp( p );
        location = data->Cursor();
    }
    const char* startTag = "<!--";
    const char* endTag   = "-->";

    if ( !StringEqual( p, startTag, false ) )
    {
        document->SetError( TIXML_ERROR_PARSING_COMMENT, p, data );
        return 0;
    }
    p += strlen( startTag );
    p = ReadText( p, &value, false, endTag, false );
    return p;
}


const char* TiXmlAttribute::Parse( const char* p, TiXmlParsingData* data )
{
    p = SkipWhiteSpace( p );
    if ( !p || !*p ) return 0;

    int tabsize = 4;
    if ( document )
        tabsize = document->TabSize();

//  TiXmlParsingData data( p, prevData );
    if ( data )
    {
        data->Stamp( p );
        location = data->Cursor();
    }
    // Read the name, the '=' and the value.
    const char* pErr = p;
    p = ReadName( p, &name );
    if ( !p || !*p )
    {
        if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, pErr, data );
        return 0;
    }
    p = SkipWhiteSpace( p );
    if ( !p || !*p || *p != '=' )
    {
        if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data );
        return 0;
    }

    ++p;    // skip '='
    p = SkipWhiteSpace( p );
    if ( !p || !*p )
    {
        if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES, p, data );
        return 0;
    }

    const char* end;

    if ( *p == '\'' )
    {
        ++p;
        end = String("\'");
        p = ReadText( p, &value, false, end, false );
    }
    else if ( *p == '"' )
    {
        ++p;
        end = String("\"");
        p = ReadText( p, &value, false, end, false );
    }
    else
    {
        // All attribute values should be in single or double quotes.
        // But this is such a common error that the parser will try
        // its best, even without them.
        value = String("");
        while (    p && *p                                      // existence
                && !iswspace( *p ) && *p != '\n' && *p != '\r' // whitespace
                && *p != '/' && *p != '>' )                     // tag end
        {
            value += *p;
            ++p;
        }
    }
    return p;
}

const char* TiXmlText::Parse( const char* p, TiXmlParsingData* data )
{
    value = "";
//  TiXmlParsingData data( p, prevData );
    if ( data )
    {
        data->Stamp( p );
        location = data->Cursor();
    }
    bool ignoreWhite = true;

    const char* end = "<";
    p = ReadText( p, &value, ignoreWhite, end, false );
    if ( p )
        return p-1; // don't truncate the '<'
    return 0;
}

const char* TiXmlDeclaration::Parse( const char* p, TiXmlParsingData* data )
{
    p = SkipWhiteSpace( p );
    // Find the beginning, find the end, and look for
    // the stuff in-between.
    TiXmlDocument* document = GetDocument();
    if ( !p || !*p || !StringEqual( p, "<?xml", true ) )
    {
        if ( document ) document->SetError( TIXML_ERROR_PARSING_DECLARATION, 0, 0 );
        return 0;
    }
//  TiXmlParsingData data( p, prevData );
    if ( data )
    {
        data->Stamp( p );
        location = data->Cursor();
    }
    p += 5;

    version = "";
    encoding = "";
    standalone = "";

    while ( p && *p )
    {
        if ( *p == '>' )
        {
            ++p;
            return p;
        }

        p = SkipWhiteSpace( p );
        if ( StringEqual( p, "version", true ) )
        {
            TiXmlAttribute attrib;
            p = attrib.Parse( p, data );
            version = attrib.Value();
        }
        else if ( StringEqual( p, "encoding", true ) )
        {
            TiXmlAttribute attrib;
            p = attrib.Parse( p, data );
            encoding = attrib.Value();
        }
        else if ( StringEqual( p, "standalone", true ) )
        {
            TiXmlAttribute attrib;
            p = attrib.Parse( p, data );
            standalone = attrib.Value();
        }
        else
        {
            // Read over whatever it is.
            while( p && *p && *p != '>' && !iswspace( *p ) )
                ++p;
        }
    }
    return 0;
}

bool TiXmlText::Blank() const
{
    for (size_t i = 0, len = value.length(); i < len; i++)
        if ( !iswspace( value[i] ) )
            return false;
    return true;
}

}