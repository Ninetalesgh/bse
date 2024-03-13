#pragma comment(lib,"user32.lib")

#include <windows.h>
#ifdef ERROR
# undef ERROR
#endif

#include <vector>
#include <string>

//moved this stuff over and quick hacking this was the easiest way.
int string_format( char* destination, int capacity, char const* value )
{
  int result = 0;
  if ( value )
  {
    while ( *value != '\0' && capacity > result )
    {
      *destination++ = *value++;
      ++result;
    }
  }
  result = min( capacity - 1, result );
  destination[result] = '\0';
  return result + 1;
}

bool string_match(char const* a, char const* b)
{
  return !strcmp(a,b);
}

char* string_copy( char* destination, char const* origin, int capacity )
{
  for ( int i = 0; i < capacity; ++i )
  {
    destination[i] = origin[i];

    if ( origin[i] == '\0' )
    {
      break;
    }
  }
  return destination;
}

char* string_copy_until( char* destination, char const* origin, int capacity, char until )
{
  for ( int i = 0; i < capacity; ++i )
  {
    if ( origin[i] != until )
    {
      destination[i] = origin[i];
    }
    else
    {
      break;
    }
  }
  return destination;
}

#define BUFFER_SIZE 16000000
#define PATH_MAX 8192

int utf8_to_wchar( char const* utf8String, wchar_t* out_wcharString, int wcharLengthMax )
{
  int wcharLength = MultiByteToWideChar( CP_UTF8, 0, utf8String, -1, 0, 0 );
  return MultiByteToWideChar( CP_UTF8, 0, utf8String, -1, out_wcharString, min( wcharLength, wcharLengthMax ) );
}

int wchar_to_utf8( wchar_t const* wcharString, char* out_utf8String, int utf8StringLengthMax )
{
  int utf8StringLength = WideCharToMultiByte( CP_UTF8, 0, wcharString, -1, 0, 0, 0, 0 );
  return WideCharToMultiByte( CP_UTF8, 0, wcharString, -1, out_utf8String, min( utf8StringLength, utf8StringLengthMax ), 0, 0 );
}

bool load_file( char const* filePath, void* targetBuffer, unsigned int bufferSize )
{
  bool result = false;
  if ( targetBuffer )
  {
    wchar_t wideChars[PATH_MAX];
    utf8_to_wchar( filePath, wideChars, PATH_MAX );
    HANDLE fileHandle = CreateFileW( wideChars,
                                     GENERIC_READ,
                                     FILE_SHARE_READ, 0,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL, 0 );

    if ( fileHandle != INVALID_HANDLE_VALUE )
    {
      DWORD bytesRead;
      if ( ReadFile( fileHandle, targetBuffer, bufferSize, &bytesRead, 0 ) )
      {
        result = true;
      }

      CloseHandle( fileHandle );
    }
  }

  return result;
}

bool write_file( char const* filePath, void const* data, unsigned int size )
{
  bool result = true;
  wchar_t wideChars[PATH_MAX];
  utf8_to_wchar( filePath, wideChars, PATH_MAX );
  HANDLE fileHandle = nullptr;

  fileHandle = CreateFileW( wideChars, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

  if ( fileHandle != INVALID_HANDLE_VALUE )
  {
    DWORD bytesWritten;
    char const* reader = (char const*) data;
    unsigned int sizeLeft = size;
    unsigned int const MAX_WRITE = 4 * 1024;

    while ( sizeLeft > 0 )
    {
      if ( WriteFile( fileHandle, reader, min( MAX_WRITE, sizeLeft ), &bytesWritten, 0 ) )
      {
        sizeLeft -= bytesWritten;
        reader += bytesWritten;
      }
      else
      {
        result = false;
        break;
      }
    }
  }
  else
  {
    result = false;
  }

  CloseHandle( fileHandle );
  return result;
}

bool exists( std::vector<std::string> const& ids, char* id )
{
  int const size = int( ids.size() );
  for ( int i = 0; i < size; ++i )
  {
    if ( ids[i] == id )
    {
      return true;
    }
  }

  return false;
}

int main( int argc, char** argv )
{
  char* wxsInputBuffer = (char*) malloc( BUFFER_SIZE );
  char* wxsOutputBuffer = (char*) malloc( BUFFER_SIZE );
  //char* wixobjInputBuffer = (char*) malloc( BUFFER_SIZE );
  //char* wixobjOutputBuffer = (char*) malloc( BUFFER_SIZE );

  std::vector<std::string> ids;
  std::vector<std::string> excludedNodes;
  std::vector<std::string> excludedIds;

  excludedNodes.push_back( "ComponentGroup" );
  excludedNodes.push_back( "DirectoryRef" );
  //excludedNodes.push_back( "File" );

  //excludedIds.push_back( "runtimes" );
  //excludedIds.push_back( "Licenses" );
  //excludedIds.push_back( "Defaults" );

  if ( argc < 2 )
  {
  error:
    printf( "give me a path to the wxsfile I should mangle IDs for.\n-wxs <filename> for input wxs, -p for custom mangle prefix." );
    return 0;
  }

  char wxsPath[8192] = {};
  //char wixobjPath[8192] = {};
  char prefix[8192] = {};

  for ( int i = 1; i < argc; ++i )
  {
    char* c = argv[i];
    if ( string_match( c, "-wxs" ) )
    {
      if ( ++i >= argc ) { goto error; }
      string_format( wxsPath, 8192, argv[i] );
    }

    // if ( string_match( c, "-wixobj" ) )
    // {
    //   if ( ++i >= argc ) { goto error; }
    //   string_format( wixobjPath, 8192, argv[i] );
    // }

    if ( string_match( c, "-p" ) || string_match( c, "-prefix" ) )
    {
      if ( ++i >= argc ) { goto error; }
      string_format( prefix, 8192, argv[i] );
    }
  }

  if ( !*wxsPath )
  {
    goto error;
  }

  // if ( !*wixobjPath )
  // {
  //   string_format( wixobjPath, 8192, wxsPath );
  //   string_format( wixobjPath + string_length( wxsPath ) - 4, 8000, ".wixobj" );
  // }

  if ( !*prefix )
  {
    string_format( prefix, 8192, "x_" );
  }

  printf( "--------------------------------------------------------------- \n" );
  printf( "Mangling wxs file: %s\n", wxsPath );
  //printf( "wixobj file: %s\n", wixobjPath );

  printf( "Excluded Nodes:\n" );
  for ( auto& str : excludedNodes )
  {
    printf( "- %s\n", str.c_str() );
  }

  if ( load_file( wxsPath, wxsInputBuffer, BUFFER_SIZE ) )
  {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf( "--------------------------------------------------------------- \n" );
    printf( "Collecting Ids... \n" );
    {
      char* reader = wxsInputBuffer;
      while ( *reader != '\0' )
      {
        if ( *reader == 'I' && reader[1] == 'd' && reader[2] == '=' && reader[3] == '\"' )
        {
          char currentNode[1024] = {};

          {
            char* ctrlRead = reader;
            while ( ctrlRead >= wxsInputBuffer && *ctrlRead != '<' ) { --ctrlRead; }
            ++ctrlRead;
            string_copy_until( currentNode, ctrlRead, 1024, ' ' );
          }

          reader += 4;

          //don't do excluded nodes
          if ( !exists( excludedNodes, currentNode ) )
          {
            char currentId[8192] = {};
            string_copy_until( currentId, reader, 8192, '\"' );

            //don't do excluded ids
            if ( !exists( excludedIds, currentId ) )
            {
              if ( !exists( ids, currentId ) )
              {
                ids.push_back( currentId );
              }
            }
          }
        }
        else
        {
          ++reader;
        }
      }

      printf( "%d Ids in total. \n", int( ids.size() ) );
      printf( "--------------------------------------------------------------- \n" );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf( "Mangling file... \n" );
    {
      char* reader = wxsInputBuffer;
      char* writer = wxsOutputBuffer;
      while ( *reader != '\0' )
      {
        if ( *reader == 'I' && reader[1] == 'd' && reader[2] == '=' && reader[3] == '\"' )
        {
          char currentNodeName[1024] = {};

          {
            char* ctrlRead = reader;
            while ( ctrlRead >= wxsInputBuffer && *ctrlRead != '<' ) { --ctrlRead; }
            ++ctrlRead;
            string_copy_until( currentNodeName, ctrlRead, 1024, ' ' );
          }

          //don't do excluded nodes
          if ( !exists( excludedNodes, currentNodeName ) )
          {
            string_copy( writer, reader, 4 );
            writer += 4;
            reader += 4;
            char currentId[8192] = {};
            string_copy_until( currentId, reader, 8192, '\"' );
            if ( exists( ids, currentId ) )
            {
              writer += string_format( writer, BUFFER_SIZE - int( writer - wxsOutputBuffer ), prefix ) - 1;
            }
          }
        }

        if ( *reader == 'D'
        && reader[1] == 'i'
        && reader[2] == 'r'
        && reader[3] == 'e'
        && reader[4] == 'c'
        && reader[5] == 't'
        && reader[6] == 'o'
        && reader[7] == 'r'
        && reader[8] == 'y'
        && reader[9] == '='
        && reader[10] == '\"' )
        {
          string_copy( writer, reader, 11 );
          writer += 11;
          reader += 11;

          //excluded nodes don't matter here so we don't check for it I think
          char currentId[8192] = {};
          string_copy_until( currentId, reader, 8192, '\"' );
          if ( exists( ids, currentId ) )
          {
            writer += string_format( writer, BUFFER_SIZE - int( writer - wxsOutputBuffer ), prefix ) - 1;
          }
        }

        *writer++ = *reader++;
      }

      write_file( wxsPath, wxsOutputBuffer, int( writer - wxsOutputBuffer ) );
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // printf( "Mangling wixobj file... \n" );
    // {
    //   char* reader = wixobjInputBuffer;
    //   char* writer = wixobjOutputBuffer;
    //   if ( load_file( wixobjPath, wixobjInputBuffer, BUFFER_SIZE ) )
    //   {
    //     while ( *reader != '\0' )
    //     {
    //       if ( *reader == '<'
    //           && reader[1] == 'f'
    //           && reader[2] == 'i'
    //           && reader[3] == 'e'
    //           && reader[4] == 'l'
    //           && reader[5] == 'd'
    //           && reader[6] == '>' )
    //       {
    //         string_copy( writer, reader, 7 );
    //         writer += 7;
    //         reader += 7;

    //         char currentId[8192] = {};
    //         string_copy_until( currentId, reader, 8192, '<' );
    //         if ( exists( ids, currentId ) )
    //         {
    //           writer += string_format( writer, BUFFER_SIZE - int( writer - wixobjOutputBuffer ), prefix ) - 1;
    //         }
    //       }
    //       *writer++ = *reader++;
    //     }

    //     write_file( wixobjPath, wixobjOutputBuffer, int( writer - wixobjOutputBuffer ) );
    //   }
    //   else
    //   {
    //     printf( "wixobj file not found!" );
    //   }
    // }
  }
  else
  {
    printf( "wxs file not found!" );
  }

  printf( "Mangle successful!\n" );
  printf( "--------------------------------------------------------------- \n" );

  return 0;
}