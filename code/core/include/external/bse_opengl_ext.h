#pragma once

#define BSE_OGLAPI __stdcall

using GLchar = char;
using GLsizeiptr = long long;
using GLintptr = intptr_t;
using GLenum = unsigned int;
using GLboolean = unsigned char;
using GLbitfield = unsigned int;
using GLbyte = signed char;
using GLshort = short;
using GLint = int;
using GLsizei = int;
using GLubyte = unsigned char;
using GLushort = unsigned short;
using GLuint = unsigned int;
using GLfloat = float;
using GLclampf = float;
using GLdouble = double;
using GLclampd = double;
using GLvoid = void;

namespace opengl_ext
{
  using get_proc_address_fn = void* BSE_OGLAPI( char const* functionName );
  GLint init( get_proc_address_fn* get_proc_address_function );
};

void check_gl_error();

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#define GL_VERSION_1_1                    1
#define GL_ACCUM                          0x0100
#define GL_LOAD                           0x0101
#define GL_RETURN                         0x0102
#define GL_MULT                           0x0103
#define GL_ADD                            0x0104
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
#define GL_CURRENT_BIT                    0x00000001
#define GL_POINT_BIT                      0x00000002
#define GL_LINE_BIT                       0x00000004
#define GL_POLYGON_BIT                    0x00000008
#define GL_POLYGON_STIPPLE_BIT            0x00000010
#define GL_PIXEL_MODE_BIT                 0x00000020
#define GL_LIGHTING_BIT                   0x00000040
#define GL_FOG_BIT                        0x00000080
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_ACCUM_BUFFER_BIT               0x00000200
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_VIEWPORT_BIT                   0x00000800
#define GL_TRANSFORM_BIT                  0x00001000
#define GL_ENABLE_BIT                     0x00002000
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_HINT_BIT                       0x00008000
#define GL_EVAL_BIT                       0x00010000
#define GL_LIST_BIT                       0x00020000
#define GL_TEXTURE_BIT                    0x00040000
#define GL_SCISSOR_BIT                    0x00080000
#define GL_ALL_ATTRIB_BITS                0x000fffff
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_TRUE                           1
#define GL_FALSE                          0
#define GL_CLIP_PLANE0                    0x3000
#define GL_CLIP_PLANE1                    0x3001
#define GL_CLIP_PLANE2                    0x3002
#define GL_CLIP_PLANE3                    0x3003
#define GL_CLIP_PLANE4                    0x3004
#define GL_CLIP_PLANE5                    0x3005
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_2_BYTES                        0x1407
#define GL_3_BYTES                        0x1408
#define GL_4_BYTES                        0x1409
#define GL_DOUBLE                         0x140A
#define GL_NONE                           0
#define GL_FRONT_LEFT                     0x0400
#define GL_FRONT_RIGHT                    0x0401
#define GL_BACK_LEFT                      0x0402
#define GL_BACK_RIGHT                     0x0403
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_LEFT                           0x0406
#define GL_RIGHT                          0x0407
#define GL_FRONT_AND_BACK                 0x0408
#define GL_AUX0                           0x0409
#define GL_AUX1                           0x040A
#define GL_AUX2                           0x040B
#define GL_AUX3                           0x040C
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_2D                             0x0600
#define GL_3D                             0x0601
#define GL_3D_COLOR                       0x0602
#define GL_3D_COLOR_TEXTURE               0x0603
#define GL_4D_COLOR_TEXTURE               0x0604
#define GL_PASS_THROUGH_TOKEN             0x0700
#define GL_POINT_TOKEN                    0x0701
#define GL_LINE_TOKEN                     0x0702
#define GL_POLYGON_TOKEN                  0x0703
#define GL_BITMAP_TOKEN                   0x0704
#define GL_DRAW_PIXEL_TOKEN               0x0705
#define GL_COPY_PIXEL_TOKEN               0x0706
#define GL_LINE_RESET_TOKEN               0x0707
#define GL_EXP                            0x0800
#define GL_EXP2                           0x0801
#define GL_CW                             0x0900
#define GL_CCW                            0x0901
#define GL_COEFF                          0x0A00
#define GL_ORDER                          0x0A01
#define GL_DOMAIN                         0x0A02
#define GL_CURRENT_COLOR                  0x0B00
#define GL_CURRENT_INDEX                  0x0B01
#define GL_CURRENT_NORMAL                 0x0B02
#define GL_CURRENT_TEXTURE_COORDS         0x0B03
#define GL_CURRENT_RASTER_COLOR           0x0B04
#define GL_CURRENT_RASTER_INDEX           0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS  0x0B06
#define GL_CURRENT_RASTER_POSITION        0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID  0x0B08
#define GL_CURRENT_RASTER_DISTANCE        0x0B09
#define GL_POINT_SMOOTH                   0x0B10
#define GL_POINT_SIZE                     0x0B11
#define GL_POINT_SIZE_RANGE               0x0B12
#define GL_POINT_SIZE_GRANULARITY         0x0B13
#define GL_LINE_SMOOTH                    0x0B20
#define GL_LINE_WIDTH                     0x0B21
#define GL_LINE_WIDTH_RANGE               0x0B22
#define GL_LINE_WIDTH_GRANULARITY         0x0B23
#define GL_LINE_STIPPLE                   0x0B24
#define GL_LINE_STIPPLE_PATTERN           0x0B25
#define GL_LINE_STIPPLE_REPEAT            0x0B26
#define GL_LIST_MODE                      0x0B30
#define GL_MAX_LIST_NESTING               0x0B31
#define GL_LIST_BASE                      0x0B32
#define GL_LIST_INDEX                     0x0B33
#define GL_POLYGON_MODE                   0x0B40
#define GL_POLYGON_SMOOTH                 0x0B41
#define GL_POLYGON_STIPPLE                0x0B42
#define GL_EDGE_FLAG                      0x0B43
#define GL_CULL_FACE                      0x0B44
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_LIGHTING                       0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER       0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE           0x0B52
#define GL_LIGHT_MODEL_AMBIENT            0x0B53
#define GL_SHADE_MODEL                    0x0B54
#define GL_COLOR_MATERIAL_FACE            0x0B55
#define GL_COLOR_MATERIAL_PARAMETER       0x0B56
#define GL_COLOR_MATERIAL                 0x0B57
#define GL_FOG                            0x0B60
#define GL_FOG_INDEX                      0x0B61
#define GL_FOG_DENSITY                    0x0B62
#define GL_FOG_START                      0x0B63
#define GL_FOG_END                        0x0B64
#define GL_FOG_MODE                       0x0B65
#define GL_FOG_COLOR                      0x0B66
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_TEST                     0x0B71
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_ACCUM_CLEAR_VALUE              0x0B80
#define GL_STENCIL_TEST                   0x0B90
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_MATRIX_MODE                    0x0BA0
#define GL_NORMALIZE                      0x0BA1
#define GL_VIEWPORT                       0x0BA2
#define GL_MODELVIEW_STACK_DEPTH          0x0BA3
#define GL_PROJECTION_STACK_DEPTH         0x0BA4
#define GL_TEXTURE_STACK_DEPTH            0x0BA5
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_PROJECTION_MATRIX              0x0BA7
#define GL_TEXTURE_MATRIX                 0x0BA8
#define GL_ATTRIB_STACK_DEPTH             0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH      0x0BB1
#define GL_ALPHA_TEST                     0x0BC0
#define GL_ALPHA_TEST_FUNC                0x0BC1
#define GL_ALPHA_TEST_REF                 0x0BC2
#define GL_DITHER                         0x0BD0
#define GL_BLEND_DST                      0x0BE0
#define GL_BLEND_SRC                      0x0BE1
#define GL_BLEND                          0x0BE2
#define GL_LOGIC_OP_MODE                  0x0BF0
#define GL_INDEX_LOGIC_OP                 0x0BF1
#define GL_COLOR_LOGIC_OP                 0x0BF2
#define GL_AUX_BUFFERS                    0x0C00
#define GL_DRAW_BUFFER                    0x0C01
#define GL_READ_BUFFER                    0x0C02
#define GL_SCISSOR_BOX                    0x0C10
#define GL_SCISSOR_TEST                   0x0C11
#define GL_INDEX_CLEAR_VALUE              0x0C20
#define GL_INDEX_WRITEMASK                0x0C21
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_INDEX_MODE                     0x0C30
#define GL_RGBA_MODE                      0x0C31
#define GL_DOUBLEBUFFER                   0x0C32
#define GL_STEREO                         0x0C33
#define GL_RENDER_MODE                    0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define GL_POINT_SMOOTH_HINT              0x0C51
#define GL_LINE_SMOOTH_HINT               0x0C52
#define GL_POLYGON_SMOOTH_HINT            0x0C53
#define GL_FOG_HINT                       0x0C54
#define GL_TEXTURE_GEN_S                  0x0C60
#define GL_TEXTURE_GEN_T                  0x0C61
#define GL_TEXTURE_GEN_R                  0x0C62
#define GL_TEXTURE_GEN_Q                  0x0C63
#define GL_PIXEL_MAP_I_TO_I               0x0C70
#define GL_PIXEL_MAP_S_TO_S               0x0C71
#define GL_PIXEL_MAP_I_TO_R               0x0C72
#define GL_PIXEL_MAP_I_TO_G               0x0C73
#define GL_PIXEL_MAP_I_TO_B               0x0C74
#define GL_PIXEL_MAP_I_TO_A               0x0C75
#define GL_PIXEL_MAP_R_TO_R               0x0C76
#define GL_PIXEL_MAP_G_TO_G               0x0C77
#define GL_PIXEL_MAP_B_TO_B               0x0C78
#define GL_PIXEL_MAP_A_TO_A               0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE          0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE          0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE          0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE          0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE          0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE          0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE          0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE          0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE          0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE          0x0CB9
#define GL_UNPACK_SWAP_BYTES              0x0CF0
#define GL_UNPACK_LSB_FIRST               0x0CF1
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_SWAP_BYTES                0x0D00
#define GL_PACK_LSB_FIRST                 0x0D01
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAP_COLOR                      0x0D10
#define GL_MAP_STENCIL                    0x0D11
#define GL_INDEX_SHIFT                    0x0D12
#define GL_INDEX_OFFSET                   0x0D13
#define GL_RED_SCALE                      0x0D14
#define GL_RED_BIAS                       0x0D15
#define GL_ZOOM_X                         0x0D16
#define GL_ZOOM_Y                         0x0D17
#define GL_GREEN_SCALE                    0x0D18
#define GL_GREEN_BIAS                     0x0D19
#define GL_BLUE_SCALE                     0x0D1A
#define GL_BLUE_BIAS                      0x0D1B
#define GL_ALPHA_SCALE                    0x0D1C
#define GL_ALPHA_BIAS                     0x0D1D
#define GL_DEPTH_SCALE                    0x0D1E
#define GL_DEPTH_BIAS                     0x0D1F
#define GL_MAX_EVAL_ORDER                 0x0D30
#define GL_MAX_LIGHTS                     0x0D31
#define GL_MAX_CLIP_PLANES                0x0D32
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_PIXEL_MAP_TABLE            0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH         0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define GL_MAX_NAME_STACK_DEPTH           0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH  0x0D3B
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_INDEX_BITS                     0x0D51
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_ACCUM_RED_BITS                 0x0D58
#define GL_ACCUM_GREEN_BITS               0x0D59
#define GL_ACCUM_BLUE_BITS                0x0D5A
#define GL_ACCUM_ALPHA_BITS               0x0D5B
#define GL_NAME_STACK_DEPTH               0x0D70
#define GL_AUTO_NORMAL                    0x0D80
#define GL_MAP1_COLOR_4                   0x0D90
#define GL_MAP1_INDEX                     0x0D91
#define GL_MAP1_NORMAL                    0x0D92
#define GL_MAP1_TEXTURE_COORD_1           0x0D93
#define GL_MAP1_TEXTURE_COORD_2           0x0D94
#define GL_MAP1_TEXTURE_COORD_3           0x0D95
#define GL_MAP1_TEXTURE_COORD_4           0x0D96
#define GL_MAP1_VERTEX_3                  0x0D97
#define GL_MAP1_VERTEX_4                  0x0D98
#define GL_MAP2_COLOR_4                   0x0DB0
#define GL_MAP2_INDEX                     0x0DB1
#define GL_MAP2_NORMAL                    0x0DB2
#define GL_MAP2_TEXTURE_COORD_1           0x0DB3
#define GL_MAP2_TEXTURE_COORD_2           0x0DB4
#define GL_MAP2_TEXTURE_COORD_3           0x0DB5
#define GL_MAP2_TEXTURE_COORD_4           0x0DB6
#define GL_MAP2_VERTEX_3                  0x0DB7
#define GL_MAP2_VERTEX_4                  0x0DB8
#define GL_MAP1_GRID_DOMAIN               0x0DD0
#define GL_MAP1_GRID_SEGMENTS             0x0DD1
#define GL_MAP2_GRID_DOMAIN               0x0DD2
#define GL_MAP2_GRID_SEGMENTS             0x0DD3
#define GL_TEXTURE_1D                     0x0DE0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER        0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE           0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE           0x0DF2
#define GL_SELECTION_BUFFER_POINTER       0x0DF3
#define GL_SELECTION_BUFFER_SIZE          0x0DF4
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT        0x1003
#define GL_TEXTURE_BORDER_COLOR           0x1004
#define GL_TEXTURE_BORDER                 0x1005
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102
#define GL_LIGHT0                         0x4000
#define GL_LIGHT1                         0x4001
#define GL_LIGHT2                         0x4002
#define GL_LIGHT3                         0x4003
#define GL_LIGHT4                         0x4004
#define GL_LIGHT5                         0x4005
#define GL_LIGHT6                         0x4006
#define GL_LIGHT7                         0x4007
#define GL_AMBIENT                        0x1200
#define GL_DIFFUSE                        0x1201
#define GL_SPECULAR                       0x1202
#define GL_POSITION                       0x1203
#define GL_SPOT_DIRECTION                 0x1204
#define GL_SPOT_EXPONENT                  0x1205
#define GL_SPOT_CUTOFF                    0x1206
#define GL_CONSTANT_ATTENUATION           0x1207
#define GL_LINEAR_ATTENUATION             0x1208
#define GL_QUADRATIC_ATTENUATION          0x1209
#define GL_COMPILE                        0x1300
#define GL_COMPILE_AND_EXECUTE            0x1301
#define GL_CLEAR                          0x1500
#define GL_AND                            0x1501
#define GL_AND_REVERSE                    0x1502
#define GL_COPY                           0x1503
#define GL_AND_INVERTED                   0x1504
#define GL_NOOP                           0x1505
#define GL_XOR                            0x1506
#define GL_OR                             0x1507
#define GL_NOR                            0x1508
#define GL_EQUIV                          0x1509
#define GL_INVERT                         0x150A
#define GL_OR_REVERSE                     0x150B
#define GL_COPY_INVERTED                  0x150C
#define GL_OR_INVERTED                    0x150D
#define GL_NAND                           0x150E
#define GL_SET                            0x150F
#define GL_EMISSION                       0x1600
#define GL_SHININESS                      0x1601
#define GL_AMBIENT_AND_DIFFUSE            0x1602
#define GL_COLOR_INDEXES                  0x1603
#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701
#define GL_TEXTURE                        0x1702
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802
#define GL_COLOR_INDEX                    0x1900
#define GL_STENCIL_INDEX                  0x1901
#define GL_DEPTH_COMPONENT                0x1902
#define GL_RED                            0x1903
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A
#define GL_BITMAP                         0x1A00
#define GL_POINT                          0x1B00
#define GL_LINE                           0x1B01
#define GL_FILL                           0x1B02
#define GL_RENDER                         0x1C00
#define GL_FEEDBACK                       0x1C01
#define GL_SELECT                         0x1C02
#define GL_FLAT                           0x1D00
#define GL_SMOOTH                         0x1D01
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_S                              0x2000
#define GL_T                              0x2001
#define GL_R                              0x2002
#define GL_Q                              0x2003
#define GL_MODULATE                       0x2100
#define GL_DECAL                          0x2101
#define GL_TEXTURE_ENV_MODE               0x2200
#define GL_TEXTURE_ENV_COLOR              0x2201
#define GL_TEXTURE_ENV                    0x2300
#define GL_EYE_LINEAR                     0x2400
#define GL_OBJECT_LINEAR                  0x2401
#define GL_SPHERE_MAP                     0x2402
#define GL_TEXTURE_GEN_MODE               0x2500
#define GL_OBJECT_PLANE                   0x2501
#define GL_EYE_PLANE                      0x2502
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_CLAMP                          0x2900
#define GL_REPEAT                         0x2901
#define GL_CLIENT_PIXEL_STORE_BIT         0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT        0x00000002
#define GL_CLIENT_ALL_ATTRIB_BITS         0xffffffff
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_POINT           0x2A01
#define GL_POLYGON_OFFSET_LINE            0x2A02
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_ALPHA4                         0x803B
#define GL_ALPHA8                         0x803C
#define GL_ALPHA12                        0x803D
#define GL_ALPHA16                        0x803E
#define GL_LUMINANCE4                     0x803F
#define GL_LUMINANCE8                     0x8040
#define GL_LUMINANCE12                    0x8041
#define GL_LUMINANCE16                    0x8042
#define GL_LUMINANCE4_ALPHA4              0x8043
#define GL_LUMINANCE6_ALPHA2              0x8044
#define GL_LUMINANCE8_ALPHA8              0x8045
#define GL_LUMINANCE12_ALPHA4             0x8046
#define GL_LUMINANCE12_ALPHA12            0x8047
#define GL_LUMINANCE16_ALPHA16            0x8048
#define GL_INTENSITY                      0x8049
#define GL_INTENSITY4                     0x804A
#define GL_INTENSITY8                     0x804B
#define GL_INTENSITY12                    0x804C
#define GL_INTENSITY16                    0x804D
#define GL_R3_G3_B2                       0x2A10
#define GL_RGB4                           0x804F
#define GL_RGB5                           0x8050
#define GL_RGB8                           0x8051
#define GL_RGB10                          0x8052
#define GL_RGB12                          0x8053
#define GL_RGB16                          0x8054
#define GL_RGBA2                          0x8055
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_RGBA12                         0x805A
#define GL_RGBA16                         0x805B
#define GL_TEXTURE_RED_SIZE               0x805C
#define GL_TEXTURE_GREEN_SIZE             0x805D
#define GL_TEXTURE_BLUE_SIZE              0x805E
#define GL_TEXTURE_ALPHA_SIZE             0x805F
#define GL_TEXTURE_LUMINANCE_SIZE         0x8060
#define GL_TEXTURE_INTENSITY_SIZE         0x8061
#define GL_PROXY_TEXTURE_1D               0x8063
#define GL_PROXY_TEXTURE_2D               0x8064
#define GL_TEXTURE_PRIORITY               0x8066
#define GL_TEXTURE_RESIDENT               0x8067
#define GL_TEXTURE_BINDING_1D             0x8068
#define GL_TEXTURE_BINDING_2D             0x8069
#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_INDEX_ARRAY                    0x8077
#define GL_TEXTURE_COORD_ARRAY            0x8078
#define GL_EDGE_FLAG_ARRAY                0x8079
#define GL_VERTEX_ARRAY_SIZE              0x807A
#define GL_VERTEX_ARRAY_TYPE              0x807B
#define GL_VERTEX_ARRAY_STRIDE            0x807C
#define GL_NORMAL_ARRAY_TYPE              0x807E
#define GL_NORMAL_ARRAY_STRIDE            0x807F
#define GL_COLOR_ARRAY_SIZE               0x8081
#define GL_COLOR_ARRAY_TYPE               0x8082
#define GL_COLOR_ARRAY_STRIDE             0x8083
#define GL_INDEX_ARRAY_TYPE               0x8085
#define GL_INDEX_ARRAY_STRIDE             0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE       0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE       0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE     0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE         0x808C
#define GL_VERTEX_ARRAY_POINTER           0x808E
#define GL_NORMAL_ARRAY_POINTER           0x808F
#define GL_COLOR_ARRAY_POINTER            0x8090
#define GL_INDEX_ARRAY_POINTER            0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER    0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER        0x8093
#define GL_V2F                            0x2A20
#define GL_V3F                            0x2A21
#define GL_C4UB_V2F                       0x2A22
#define GL_C4UB_V3F                       0x2A23
#define GL_C3F_V3F                        0x2A24
#define GL_N3F_V3F                        0x2A25
#define GL_C4F_N3F_V3F                    0x2A26
#define GL_T2F_V3F                        0x2A27
#define GL_T4F_V4F                        0x2A28
#define GL_T2F_C4UB_V3F                   0x2A29
#define GL_T2F_C3F_V3F                    0x2A2A
#define GL_T2F_N3F_V3F                    0x2A2B
#define GL_T2F_C4F_N3F_V3F                0x2A2C
#define GL_T4F_C4F_N3F_V4F                0x2A2D
#define GL_EXT_vertex_array               1
#define GL_EXT_bgra                       1
#define GL_EXT_paletted_texture           1
#define GL_WIN_swap_hint                  1
#define GL_WIN_draw_range_elements        1
#define GL_VERTEX_ARRAY_EXT               0x8074
#define GL_NORMAL_ARRAY_EXT               0x8075
#define GL_COLOR_ARRAY_EXT                0x8076
#define GL_INDEX_ARRAY_EXT                0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT        0x8078
#define GL_EDGE_FLAG_ARRAY_EXT            0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT          0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT          0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT        0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT         0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT          0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT        0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT         0x8080
#define GL_COLOR_ARRAY_SIZE_EXT           0x8081
#define GL_COLOR_ARRAY_TYPE_EXT           0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT         0x8083
#define GL_COLOR_ARRAY_COUNT_EXT          0x8084
#define GL_INDEX_ARRAY_TYPE_EXT           0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT         0x8086
#define GL_INDEX_ARRAY_COUNT_EXT          0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT   0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT   0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT  0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT     0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT      0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT       0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT       0x808F
#define GL_COLOR_ARRAY_POINTER_EXT        0x8090
#define GL_INDEX_ARRAY_POINTER_EXT        0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT    0x8093
#define GL_DOUBLE_EXT                     GL_DOUBLE
#define GL_BGR_EXT                        0x80E0
#define GL_BGRA_EXT                       0x80E1
#define GL_COLOR_TABLE_FORMAT_EXT         0x80D8
#define GL_COLOR_TABLE_WIDTH_EXT          0x80D9
#define GL_COLOR_TABLE_RED_SIZE_EXT       0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_EXT     0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_EXT      0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_EXT     0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_EXT 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_EXT 0x80DF
#define GL_COLOR_INDEX1_EXT               0x80E2
#define GL_COLOR_INDEX2_EXT               0x80E3
#define GL_COLOR_INDEX4_EXT               0x80E4
#define GL_COLOR_INDEX8_EXT               0x80E5
#define GL_COLOR_INDEX12_EXT              0x80E6
#define GL_COLOR_INDEX16_EXT              0x80E7
#define GL_MAX_ELEMENTS_VERTICES_WIN      0x80E8
#define GL_MAX_ELEMENTS_INDICES_WIN       0x80E9
#define GL_PHONG_WIN                      0x80EA 
#define GL_PHONG_HINT_WIN                 0x80EB 
#define GL_FOG_SPECULAR_TEXTURE_WIN       0x80EC
#define GL_LOGIC_OP GL_INDEX_LOGIC_OP
#define GL_TEXTURE_COMPONENTS GL_TEXTURE_INTERNAL_FORMAT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// extension stuff ///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP         0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_COMPRESSED_RGB                 0x84ED
#define GL_COMPRESSED_RGBA                0x84EE
#define GL_TEXTURE_COMPRESSION_HINT       0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_CLAMP_TO_BORDER                0x812D

#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7
#define GL_MIRRORED_REPEAT                0x8370
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_TEXTURE_DEPTH_SIZE             0x884A
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_BLEND_COLOR                    0x8005
#define GL_BLEND_EQUATION                 0x8009
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_FUNC_ADD                       0x8006
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008

#define GL_OUT_OF_MEMORY                  0x0505
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84

#define GL_ARRAY_BUFFER                   0x8892
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA

#define GL_SHADER_TYPE                    0x8B4F
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_FLOAT_MAT2x3                   0x8B65
#define GL_FLOAT_MAT2x4                   0x8B66
#define GL_FLOAT_MAT3x2                   0x8B67
#define GL_FLOAT_MAT3x4                   0x8B68
#define GL_FLOAT_MAT4x2                   0x8B69
#define GL_FLOAT_MAT4x3                   0x8B6A
#define GL_SAMPLER_1D                     0x8B5D
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_SAMPLER_1D_SHADOW              0x8B61
#define GL_SAMPLER_2D_SHADOW              0x8B62

#define GL_RGBA8                          0x8058
#define GL_SRGB                           0x8C40
#define GL_SRGB_ALPHA                     0x8C42
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_FRAMEBUFFER_SRGB               0x8DB9

#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_UNIFORM_BUFFER                              0x8A11
#define GL_UNIFORM_BUFFER_BINDING                      0x8A28
#define GL_UNIFORM_BUFFER_START                        0x8A29
#define GL_UNIFORM_BUFFER_SIZE                         0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS                   0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS                 0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS                 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS                 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS                 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE                      0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS      0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS    0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS    0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT             0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH        0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS                       0x8A36
#define GL_UNIFORM_TYPE                                0x8A37
#define GL_UNIFORM_SIZE                                0x8A38
#define GL_UNIFORM_NAME_LENGTH                         0x8A39
#define GL_UNIFORM_BLOCK_INDEX                         0x8A3A
#define GL_UNIFORM_OFFSET                              0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE                        0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE                       0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR                        0x8A3E
#define GL_UNIFORM_BLOCK_BINDING                       0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE                     0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH                   0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS               0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES        0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER   0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX                               0xFFFFFFFFu

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB  0x20A9
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_RED_BITS_ARB                  0x2015
#define WGL_RED_SHIFT_ARB                 0x2016
#define WGL_GREEN_BITS_ARB                0x2017
#define WGL_GREEN_SHIFT_ARB               0x2018
#define WGL_BLUE_BITS_ARB                 0x2019
#define WGL_BLUE_SHIFT_ARB                0x201A
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_ALPHA_SHIFT_ARB               0x201C
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_VERSION_ARB               0x2095
#define ERROR_INVALID_PROFILE_ARB               0x2096

#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A

#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM     0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER   0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY       0x8249
#define GL_DEBUG_SOURCE_APPLICATION       0x824A
#define GL_DEBUG_SOURCE_OTHER             0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B

namespace opengl_ext
{
  // OPENGL 1.1 ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  using glAccum                  = void      BSE_OGLAPI( GLenum op, GLfloat value );
  using glAlphaFunc              = void      BSE_OGLAPI( GLenum func, GLclampf ref );
  using glAreTexturesResident    = GLboolean BSE_OGLAPI( GLsizei n, const GLuint* textures, GLboolean* residences );
  using glArrayElement           = void      BSE_OGLAPI( GLint i );
  using glBegin                  = void      BSE_OGLAPI( GLenum mode );
  using glBindTexture            = void      BSE_OGLAPI( GLenum target, GLuint texture );
  using glBitmap                 = void      BSE_OGLAPI( GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte* bitmap );
  using glBlendFunc              = void      BSE_OGLAPI( GLenum sfactor, GLenum dfactor );
  using glCallList               = void      BSE_OGLAPI( GLuint list );
  using glCallLists              = void      BSE_OGLAPI( GLsizei n, GLenum type, const GLvoid* lists );
  using glClear                  = void      BSE_OGLAPI( GLbitfield mask );
  using glClearAccum             = void      BSE_OGLAPI( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
  using glClearColor             = void      BSE_OGLAPI( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
  using glClearDepth             = void      BSE_OGLAPI( GLclampd depth );
  using glClearIndex             = void      BSE_OGLAPI( GLfloat c );
  using glClearStencil           = void      BSE_OGLAPI( GLint s );
  using glClipPlane              = void      BSE_OGLAPI( GLenum plane, const GLdouble* equation );
  using glColor3b                = void      BSE_OGLAPI( GLbyte red, GLbyte green, GLbyte blue );
  using glColor3bv               = void      BSE_OGLAPI( const GLbyte* v );
  using glColor3d                = void      BSE_OGLAPI( GLdouble red, GLdouble green, GLdouble blue );
  using glColor3dv               = void      BSE_OGLAPI( const GLdouble* v );
  using glColor3f                = void      BSE_OGLAPI( GLfloat red, GLfloat green, GLfloat blue );
  using glColor3fv               = void      BSE_OGLAPI( const GLfloat* v );
  using glColor3i                = void      BSE_OGLAPI( GLint red, GLint green, GLint blue );
  using glColor3iv               = void      BSE_OGLAPI( const GLint* v );
  using glColor3s                = void      BSE_OGLAPI( GLshort red, GLshort green, GLshort blue );
  using glColor3sv               = void      BSE_OGLAPI( const GLshort* v );
  using glColor3ub               = void      BSE_OGLAPI( GLubyte red, GLubyte green, GLubyte blue );
  using glColor3ubv              = void      BSE_OGLAPI( const GLubyte* v );
  using glColor3ui               = void      BSE_OGLAPI( GLuint red, GLuint green, GLuint blue );
  using glColor3uiv              = void      BSE_OGLAPI( const GLuint* v );
  using glColor3us               = void      BSE_OGLAPI( GLushort red, GLushort green, GLushort blue );
  using glColor3usv              = void      BSE_OGLAPI( const GLushort* v );
  using glColor4b                = void      BSE_OGLAPI( GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha );
  using glColor4bv               = void      BSE_OGLAPI( const GLbyte* v );
  using glColor4d                = void      BSE_OGLAPI( GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha );
  using glColor4dv               = void      BSE_OGLAPI( const GLdouble* v );
  using glColor4f                = void      BSE_OGLAPI( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
  using glColor4fv               = void      BSE_OGLAPI( const GLfloat* v );
  using glColor4i                = void      BSE_OGLAPI( GLint red, GLint green, GLint blue, GLint alpha );
  using glColor4iv               = void      BSE_OGLAPI( const GLint* v );
  using glColor4s                = void      BSE_OGLAPI( GLshort red, GLshort green, GLshort blue, GLshort alpha );
  using glColor4sv               = void      BSE_OGLAPI( const GLshort* v );
  using glColor4ub               = void      BSE_OGLAPI( GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha );
  using glColor4ubv              = void      BSE_OGLAPI( const GLubyte* v );
  using glColor4ui               = void      BSE_OGLAPI( GLuint red, GLuint green, GLuint blue, GLuint alpha );
  using glColor4uiv              = void      BSE_OGLAPI( const GLuint* v );
  using glColor4us               = void      BSE_OGLAPI( GLushort red, GLushort green, GLushort blue, GLushort alpha );
  using glColor4usv              = void      BSE_OGLAPI( const GLushort* v );
  using glColorMask              = void      BSE_OGLAPI( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
  using glColorMaterial          = void      BSE_OGLAPI( GLenum face, GLenum mode );
  using glColorPointer           = void      BSE_OGLAPI( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  using glCopyPixels             = void      BSE_OGLAPI( GLint x, GLint y, GLsizei width, GLsizei height, GLenum type );
  using glCopyTexImage1D         = void      BSE_OGLAPI( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border );
  using glCopyTexImage2D         = void      BSE_OGLAPI( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border );
  using glCopyTexSubImage1D      = void      BSE_OGLAPI( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width );
  using glCopyTexSubImage2D      = void      BSE_OGLAPI( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height );
  using glCullFace               = void      BSE_OGLAPI( GLenum mode );
  using glDeleteLists            = void      BSE_OGLAPI( GLuint list, GLsizei range );
  using glDeleteTextures         = void      BSE_OGLAPI( GLsizei n, const GLuint* textures );
  using glDepthFunc              = void      BSE_OGLAPI( GLenum func );
  using glDepthMask              = void      BSE_OGLAPI( GLboolean flag );
  using glDepthRange             = void      BSE_OGLAPI( GLclampd zNear, GLclampd zFar );
  using glDisable                = void      BSE_OGLAPI( GLenum cap );
  using glDisableClientState     = void      BSE_OGLAPI( GLenum array );
  using glDrawArrays             = void      BSE_OGLAPI( GLenum mode, GLint first, GLsizei count );
  using glDrawBuffer             = void      BSE_OGLAPI( GLenum mode );
  using glDrawElements           = void      BSE_OGLAPI( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
  using glDrawPixels             = void      BSE_OGLAPI( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels );
  using glEdgeFlag               = void      BSE_OGLAPI( GLboolean flag );
  using glEdgeFlagPointer        = void      BSE_OGLAPI( GLsizei stride, const GLvoid* pointer );
  using glEdgeFlagv              = void      BSE_OGLAPI( const GLboolean* flag );
  using glEnable                 = void      BSE_OGLAPI( GLenum cap );
  using glEnableClientState      = void      BSE_OGLAPI( GLenum array );
  using glEnd                    = void      BSE_OGLAPI( void );
  using glEndList                = void      BSE_OGLAPI( void );
  using glEvalCoord1d            = void      BSE_OGLAPI( GLdouble u );
  using glEvalCoord1dv           = void      BSE_OGLAPI( const GLdouble* u );
  using glEvalCoord1f            = void      BSE_OGLAPI( GLfloat u );
  using glEvalCoord1fv           = void      BSE_OGLAPI( const GLfloat* u );
  using glEvalCoord2d            = void      BSE_OGLAPI( GLdouble u, GLdouble v );
  using glEvalCoord2dv           = void      BSE_OGLAPI( const GLdouble* u );
  using glEvalCoord2f            = void      BSE_OGLAPI( GLfloat u, GLfloat v );
  using glEvalCoord2fv           = void      BSE_OGLAPI( const GLfloat* u );
  using glEvalMesh1              = void      BSE_OGLAPI( GLenum mode, GLint i1, GLint i2 );
  using glEvalMesh2              = void      BSE_OGLAPI( GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2 );
  using glEvalPoint1             = void      BSE_OGLAPI( GLint i );
  using glEvalPoint2             = void      BSE_OGLAPI( GLint i, GLint j );
  using glFeedbackBuffer         = void      BSE_OGLAPI( GLsizei size, GLenum type, GLfloat* buffer );
  using glFinish                 = void      BSE_OGLAPI( void );
  using glFlush                  = void      BSE_OGLAPI( void );
  using glFogf                   = void      BSE_OGLAPI( GLenum pname, GLfloat param );
  using glFogfv                  = void      BSE_OGLAPI( GLenum pname, const GLfloat* params );
  using glFogi                   = void      BSE_OGLAPI( GLenum pname, GLint param );
  using glFogiv                  = void      BSE_OGLAPI( GLenum pname, const GLint* params );
  using glFrontFace              = void      BSE_OGLAPI( GLenum mode );
  using glFrustum                = void      BSE_OGLAPI( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
  using glGenLists               = GLuint    BSE_OGLAPI( GLsizei range );
  using glGenTextures            = void      BSE_OGLAPI( GLsizei n, GLuint* textures );
  using glGetBooleanv            = void      BSE_OGLAPI( GLenum pname, GLboolean* params );
  using glGetClipPlane           = void      BSE_OGLAPI( GLenum plane, GLdouble* equation );
  using glGetDoublev             = void      BSE_OGLAPI( GLenum pname, GLdouble* params );
  using glGetError               = GLenum    BSE_OGLAPI( void );
  using glGetFloatv              = void      BSE_OGLAPI( GLenum pname, GLfloat* params );
  using glGetIntegerv            = void      BSE_OGLAPI( GLenum pname, GLint* params );
  using glGetLightfv             = void      BSE_OGLAPI( GLenum light, GLenum pname, GLfloat* params );
  using glGetLightiv             = void      BSE_OGLAPI( GLenum light, GLenum pname, GLint* params );
  using glGetMapdv               = void      BSE_OGLAPI( GLenum target, GLenum query, GLdouble* v );
  using glGetMapfv               = void      BSE_OGLAPI( GLenum target, GLenum query, GLfloat* v );
  using glGetMapiv               = void      BSE_OGLAPI( GLenum target, GLenum query, GLint* v );
  using glGetMaterialfv          = void      BSE_OGLAPI( GLenum face, GLenum pname, GLfloat* params );
  using glGetMaterialiv          = void      BSE_OGLAPI( GLenum face, GLenum pname, GLint* params );
  using glGetPixelMapfv          = void      BSE_OGLAPI( GLenum map, GLfloat* values );
  using glGetPixelMapuiv         = void      BSE_OGLAPI( GLenum map, GLuint* values );
  using glGetPixelMapusv         = void      BSE_OGLAPI( GLenum map, GLushort* values );
  using glGetPointerv            = void      BSE_OGLAPI( GLenum pname, GLvoid** params );
  using glGetPolygonStipple      = void      BSE_OGLAPI( GLubyte* mask );
  using glGetString              = const     GLubyte* BSE_OGLAPI( GLenum name );
  using glGetTexEnvfv            = void      BSE_OGLAPI( GLenum target, GLenum pname, GLfloat* params );
  using glGetTexEnviv            = void      BSE_OGLAPI( GLenum target, GLenum pname, GLint* params );
  using glGetTexGendv            = void      BSE_OGLAPI( GLenum coord, GLenum pname, GLdouble* params );
  using glGetTexGenfv            = void      BSE_OGLAPI( GLenum coord, GLenum pname, GLfloat* params );
  using glGetTexGeniv            = void      BSE_OGLAPI( GLenum coord, GLenum pname, GLint* params );
  using glGetTexImage            = void      BSE_OGLAPI( GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels );
  using glGetTexLevelParameterfv = void      BSE_OGLAPI( GLenum target, GLint level, GLenum pname, GLfloat* params );
  using glGetTexLevelParameteriv = void      BSE_OGLAPI( GLenum target, GLint level, GLenum pname, GLint* params );
  using glGetTexParameterfv      = void      BSE_OGLAPI( GLenum target, GLenum pname, GLfloat* params );
  using glGetTexParameteriv      = void      BSE_OGLAPI( GLenum target, GLenum pname, GLint* params );
  using glHint                   = void      BSE_OGLAPI( GLenum target, GLenum mode );
  using glIndexMask              = void      BSE_OGLAPI( GLuint mask );
  using glIndexPointer           = void      BSE_OGLAPI( GLenum type, GLsizei stride, const GLvoid* pointer );
  using glIndexd                 = void      BSE_OGLAPI( GLdouble c );
  using glIndexdv                = void      BSE_OGLAPI( const GLdouble* c );
  using glIndexf                 = void      BSE_OGLAPI( GLfloat c );
  using glIndexfv                = void      BSE_OGLAPI( const GLfloat* c );
  using glIndexi                 = void      BSE_OGLAPI( GLint c );
  using glIndexiv                = void      BSE_OGLAPI( const GLint* c );
  using glIndexs                 = void      BSE_OGLAPI( GLshort c );
  using glIndexsv                = void      BSE_OGLAPI( const GLshort* c );
  using glIndexub                = void      BSE_OGLAPI( GLubyte c );
  using glIndexubv               = void      BSE_OGLAPI( const GLubyte* c );
  using glInitNames              = void      BSE_OGLAPI( void );
  using glInterleavedArrays      = void      BSE_OGLAPI( GLenum format, GLsizei stride, const GLvoid* pointer );
  using glIsEnabled              = GLboolean BSE_OGLAPI( GLenum cap );
  using glIsList                 = GLboolean BSE_OGLAPI( GLuint list );
  using glIsTexture              = GLboolean BSE_OGLAPI( GLuint texture );
  using glLightModelf            = void      BSE_OGLAPI( GLenum pname, GLfloat param );
  using glLightModelfv           = void      BSE_OGLAPI( GLenum pname, const GLfloat* params );
  using glLightModeli            = void      BSE_OGLAPI( GLenum pname, GLint param );
  using glLightModeliv           = void      BSE_OGLAPI( GLenum pname, const GLint* params );
  using glLightf                 = void      BSE_OGLAPI( GLenum light, GLenum pname, GLfloat param );
  using glLightfv                = void      BSE_OGLAPI( GLenum light, GLenum pname, const GLfloat* params );
  using glLighti                 = void      BSE_OGLAPI( GLenum light, GLenum pname, GLint param );
  using glLightiv                = void      BSE_OGLAPI( GLenum light, GLenum pname, const GLint* params );
  using glLineStipple            = void      BSE_OGLAPI( GLint factor, GLushort pattern );
  using glLineWidth              = void      BSE_OGLAPI( GLfloat width );
  using glListBase               = void      BSE_OGLAPI( GLuint base );
  using glLoadIdentity           = void      BSE_OGLAPI( void );
  using glLoadMatrixd            = void      BSE_OGLAPI( const GLdouble* m );
  using glLoadMatrixf            = void      BSE_OGLAPI( const GLfloat* m );
  using glLoadName               = void      BSE_OGLAPI( GLuint name );
  using glLogicOp                = void      BSE_OGLAPI( GLenum opcode );
  using glMap1d                  = void      BSE_OGLAPI( GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points );
  using glMap1f                  = void      BSE_OGLAPI( GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points );
  using glMap2d                  = void      BSE_OGLAPI( GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points );
  using glMap2f                  = void      BSE_OGLAPI( GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points );
  using glMapGrid1d              = void      BSE_OGLAPI( GLint un, GLdouble u1, GLdouble u2 );
  using glMapGrid1f              = void      BSE_OGLAPI( GLint un, GLfloat u1, GLfloat u2 );
  using glMapGrid2d              = void      BSE_OGLAPI( GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2 );
  using glMapGrid2f              = void      BSE_OGLAPI( GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2 );
  using glMaterialf              = void      BSE_OGLAPI( GLenum face, GLenum pname, GLfloat param );
  using glMaterialfv             = void      BSE_OGLAPI( GLenum face, GLenum pname, const GLfloat* params );
  using glMateriali              = void      BSE_OGLAPI( GLenum face, GLenum pname, GLint param );
  using glMaterialiv             = void      BSE_OGLAPI( GLenum face, GLenum pname, const GLint* params );
  using glMatrixMode             = void      BSE_OGLAPI( GLenum mode );
  using glMultMatrixd            = void      BSE_OGLAPI( const GLdouble* m );
  using glMultMatrixf            = void      BSE_OGLAPI( const GLfloat* m );
  using glNewList                = void      BSE_OGLAPI( GLuint list, GLenum mode );
  using glNormal3b               = void      BSE_OGLAPI( GLbyte nx, GLbyte ny, GLbyte nz );
  using glNormal3bv              = void      BSE_OGLAPI( const GLbyte* v );
  using glNormal3d               = void      BSE_OGLAPI( GLdouble nx, GLdouble ny, GLdouble nz );
  using glNormal3dv              = void      BSE_OGLAPI( const GLdouble* v );
  using glNormal3f               = void      BSE_OGLAPI( GLfloat nx, GLfloat ny, GLfloat nz );
  using glNormal3fv              = void      BSE_OGLAPI( const GLfloat* v );
  using glNormal3i               = void      BSE_OGLAPI( GLint nx, GLint ny, GLint nz );
  using glNormal3iv              = void      BSE_OGLAPI( const GLint* v );
  using glNormal3s               = void      BSE_OGLAPI( GLshort nx, GLshort ny, GLshort nz );
  using glNormal3sv              = void      BSE_OGLAPI( const GLshort* v );
  using glNormalPointer          = void      BSE_OGLAPI( GLenum type, GLsizei stride, const GLvoid* pointer );
  using glOrtho                  = void      BSE_OGLAPI( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
  using glPassThrough            = void      BSE_OGLAPI( GLfloat token );
  using glPixelMapfv             = void      BSE_OGLAPI( GLenum map, GLsizei mapsize, const GLfloat* values );
  using glPixelMapuiv            = void      BSE_OGLAPI( GLenum map, GLsizei mapsize, const GLuint* values );
  using glPixelMapusv            = void      BSE_OGLAPI( GLenum map, GLsizei mapsize, const GLushort* values );
  using glPixelStoref            = void      BSE_OGLAPI( GLenum pname, GLfloat param );
  using glPixelStorei            = void      BSE_OGLAPI( GLenum pname, GLint param );
  using glPixelTransferf         = void      BSE_OGLAPI( GLenum pname, GLfloat param );
  using glPixelTransferi         = void      BSE_OGLAPI( GLenum pname, GLint param );
  using glPixelZoom              = void      BSE_OGLAPI( GLfloat xfactor, GLfloat yfactor );
  using glPointSize              = void      BSE_OGLAPI( GLfloat size );
  using glPolygonMode            = void      BSE_OGLAPI( GLenum face, GLenum mode );
  using glPolygonOffset          = void      BSE_OGLAPI( GLfloat factor, GLfloat units );
  using glPolygonStipple         = void      BSE_OGLAPI( const GLubyte* mask );
  using glPopAttrib              = void      BSE_OGLAPI( void );
  using glPopClientAttrib        = void      BSE_OGLAPI( void );
  using glPopMatrix              = void      BSE_OGLAPI( void );
  using glPopName                = void      BSE_OGLAPI( void );
  using glPrioritizeTextures     = void      BSE_OGLAPI( GLsizei n, const GLuint* textures, const GLclampf* priorities );
  using glPushAttrib             = void      BSE_OGLAPI( GLbitfield mask );
  using glPushClientAttrib       = void      BSE_OGLAPI( GLbitfield mask );
  using glPushMatrix             = void      BSE_OGLAPI( void );
  using glPushName               = void      BSE_OGLAPI( GLuint name );
  using glRasterPos2d            = void      BSE_OGLAPI( GLdouble x, GLdouble y );
  using glRasterPos2dv           = void      BSE_OGLAPI( const GLdouble* v );
  using glRasterPos2f            = void      BSE_OGLAPI( GLfloat x, GLfloat y );
  using glRasterPos2fv           = void      BSE_OGLAPI( const GLfloat* v );
  using glRasterPos2i            = void      BSE_OGLAPI( GLint x, GLint y );
  using glRasterPos2iv           = void      BSE_OGLAPI( const GLint* v );
  using glRasterPos2s            = void      BSE_OGLAPI( GLshort x, GLshort y );
  using glRasterPos2sv           = void      BSE_OGLAPI( const GLshort* v );
  using glRasterPos3d            = void      BSE_OGLAPI( GLdouble x, GLdouble y, GLdouble z );
  using glRasterPos3dv           = void      BSE_OGLAPI( const GLdouble* v );
  using glRasterPos3f            = void      BSE_OGLAPI( GLfloat x, GLfloat y, GLfloat z );
  using glRasterPos3fv           = void      BSE_OGLAPI( const GLfloat* v );
  using glRasterPos3i            = void      BSE_OGLAPI( GLint x, GLint y, GLint z );
  using glRasterPos3iv           = void      BSE_OGLAPI( const GLint* v );
  using glRasterPos3s            = void      BSE_OGLAPI( GLshort x, GLshort y, GLshort z );
  using glRasterPos3sv           = void      BSE_OGLAPI( const GLshort* v );
  using glRasterPos4d            = void      BSE_OGLAPI( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
  using glRasterPos4dv           = void      BSE_OGLAPI( const GLdouble* v );
  using glRasterPos4f            = void      BSE_OGLAPI( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
  using glRasterPos4fv           = void      BSE_OGLAPI( const GLfloat* v );
  using glRasterPos4i            = void      BSE_OGLAPI( GLint x, GLint y, GLint z, GLint w );
  using glRasterPos4iv           = void      BSE_OGLAPI( const GLint* v );
  using glRasterPos4s            = void      BSE_OGLAPI( GLshort x, GLshort y, GLshort z, GLshort w );
  using glRasterPos4sv           = void      BSE_OGLAPI( const GLshort* v );
  using glReadBuffer             = void      BSE_OGLAPI( GLenum mode );
  using glReadPixels             = void      BSE_OGLAPI( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels );
  using glRectd                  = void      BSE_OGLAPI( GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2 );
  using glRectdv                 = void      BSE_OGLAPI( const GLdouble* v1, const GLdouble* v2 );
  using glRectf                  = void      BSE_OGLAPI( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
  using glRectfv                 = void      BSE_OGLAPI( const GLfloat* v1, const GLfloat* v2 );
  using glRecti                  = void      BSE_OGLAPI( GLint x1, GLint y1, GLint x2, GLint y2 );
  using glRectiv                 = void      BSE_OGLAPI( const GLint* v1, const GLint* v2 );
  using glRects                  = void      BSE_OGLAPI( GLshort x1, GLshort y1, GLshort x2, GLshort y2 );
  using glRectsv                 = void      BSE_OGLAPI( const GLshort* v1, const GLshort* v2 );
  using glRenderMode             = GLint     BSE_OGLAPI( GLenum mode );
  using glRotated                = void      BSE_OGLAPI( GLdouble angle, GLdouble x, GLdouble y, GLdouble z );
  using glRotatef                = void      BSE_OGLAPI( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
  using glScaled                 = void      BSE_OGLAPI( GLdouble x, GLdouble y, GLdouble z );
  using glScalef                 = void      BSE_OGLAPI( GLfloat x, GLfloat y, GLfloat z );
  using glScissor                = void      BSE_OGLAPI( GLint x, GLint y, GLsizei width, GLsizei height );
  using glSelectBuffer           = void      BSE_OGLAPI( GLsizei size, GLuint* buffer );
  using glShadeModel             = void      BSE_OGLAPI( GLenum mode );
  using glStencilFunc            = void      BSE_OGLAPI( GLenum func, GLint ref, GLuint mask );
  using glStencilMask            = void      BSE_OGLAPI( GLuint mask );
  using glStencilOp              = void      BSE_OGLAPI( GLenum fail, GLenum zfail, GLenum zpass );
  using glTexCoord1d             = void      BSE_OGLAPI( GLdouble s );
  using glTexCoord1dv            = void      BSE_OGLAPI( const GLdouble* v );
  using glTexCoord1f             = void      BSE_OGLAPI( GLfloat s );
  using glTexCoord1fv            = void      BSE_OGLAPI( const GLfloat* v );
  using glTexCoord1i             = void      BSE_OGLAPI( GLint s );
  using glTexCoord1iv            = void      BSE_OGLAPI( const GLint* v );
  using glTexCoord1s             = void      BSE_OGLAPI( GLshort s );
  using glTexCoord1sv            = void      BSE_OGLAPI( const GLshort* v );
  using glTexCoord2d             = void      BSE_OGLAPI( GLdouble s, GLdouble t );
  using glTexCoord2dv            = void      BSE_OGLAPI( const GLdouble* v );
  using glTexCoord2f             = void      BSE_OGLAPI( GLfloat s, GLfloat t );
  using glTexCoord2fv            = void      BSE_OGLAPI( const GLfloat* v );
  using glTexCoord2i             = void      BSE_OGLAPI( GLint s, GLint t );
  using glTexCoord2iv            = void      BSE_OGLAPI( const GLint* v );
  using glTexCoord2s             = void      BSE_OGLAPI( GLshort s, GLshort t );
  using glTexCoord2sv            = void      BSE_OGLAPI( const GLshort* v );
  using glTexCoord3d             = void      BSE_OGLAPI( GLdouble s, GLdouble t, GLdouble r );
  using glTexCoord3dv            = void      BSE_OGLAPI( const GLdouble* v );
  using glTexCoord3f             = void      BSE_OGLAPI( GLfloat s, GLfloat t, GLfloat r );
  using glTexCoord3fv            = void      BSE_OGLAPI( const GLfloat* v );
  using glTexCoord3i             = void      BSE_OGLAPI( GLint s, GLint t, GLint r );
  using glTexCoord3iv            = void      BSE_OGLAPI( const GLint* v );
  using glTexCoord3s             = void      BSE_OGLAPI( GLshort s, GLshort t, GLshort r );
  using glTexCoord3sv            = void      BSE_OGLAPI( const GLshort* v );
  using glTexCoord4d             = void      BSE_OGLAPI( GLdouble s, GLdouble t, GLdouble r, GLdouble q );
  using glTexCoord4dv            = void      BSE_OGLAPI( const GLdouble* v );
  using glTexCoord4f             = void      BSE_OGLAPI( GLfloat s, GLfloat t, GLfloat r, GLfloat q );
  using glTexCoord4fv            = void      BSE_OGLAPI( const GLfloat* v );
  using glTexCoord4i             = void      BSE_OGLAPI( GLint s, GLint t, GLint r, GLint q );
  using glTexCoord4iv            = void      BSE_OGLAPI( const GLint* v );
  using glTexCoord4s             = void      BSE_OGLAPI( GLshort s, GLshort t, GLshort r, GLshort q );
  using glTexCoord4sv            = void      BSE_OGLAPI( const GLshort* v );
  using glTexCoordPointer        = void      BSE_OGLAPI( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  using glTexEnvf                = void      BSE_OGLAPI( GLenum target, GLenum pname, GLfloat param );
  using glTexEnvfv               = void      BSE_OGLAPI( GLenum target, GLenum pname, const GLfloat* params );
  using glTexEnvi                = void      BSE_OGLAPI( GLenum target, GLenum pname, GLint param );
  using glTexEnviv               = void      BSE_OGLAPI( GLenum target, GLenum pname, const GLint* params );
  using glTexGend                = void      BSE_OGLAPI( GLenum coord, GLenum pname, GLdouble param );
  using glTexGendv               = void      BSE_OGLAPI( GLenum coord, GLenum pname, const GLdouble* params );
  using glTexGenf                = void      BSE_OGLAPI( GLenum coord, GLenum pname, GLfloat param );
  using glTexGenfv               = void      BSE_OGLAPI( GLenum coord, GLenum pname, const GLfloat* params );
  using glTexGeni                = void      BSE_OGLAPI( GLenum coord, GLenum pname, GLint param );
  using glTexGeniv               = void      BSE_OGLAPI( GLenum coord, GLenum pname, const GLint* params );
  using glTexImage1D             = void      BSE_OGLAPI( GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels );
  using glTexImage2D             = void      BSE_OGLAPI( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels );
  using glTexParameterf          = void      BSE_OGLAPI( GLenum target, GLenum pname, GLfloat param );
  using glTexParameterfv         = void      BSE_OGLAPI( GLenum target, GLenum pname, const GLfloat* params );
  using glTexParameteri          = void      BSE_OGLAPI( GLenum target, GLenum pname, GLint param );
  using glTexParameteriv         = void      BSE_OGLAPI( GLenum target, GLenum pname, const GLint* params );
  using glTexSubImage1D          = void      BSE_OGLAPI( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels );
  using glTexSubImage2D          = void      BSE_OGLAPI( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels );
  using glTranslated             = void      BSE_OGLAPI( GLdouble x, GLdouble y, GLdouble z );
  using glTranslatef             = void      BSE_OGLAPI( GLfloat x, GLfloat y, GLfloat z );
  using glVertex2d               = void      BSE_OGLAPI( GLdouble x, GLdouble y );
  using glVertex2dv              = void      BSE_OGLAPI( const GLdouble* v );
  using glVertex2f               = void      BSE_OGLAPI( GLfloat x, GLfloat y );
  using glVertex2fv              = void      BSE_OGLAPI( const GLfloat* v );
  using glVertex2i               = void      BSE_OGLAPI( GLint x, GLint y );
  using glVertex2iv              = void      BSE_OGLAPI( const GLint* v );
  using glVertex2s               = void      BSE_OGLAPI( GLshort x, GLshort y );
  using glVertex2sv              = void      BSE_OGLAPI( const GLshort* v );
  using glVertex3d               = void      BSE_OGLAPI( GLdouble x, GLdouble y, GLdouble z );
  using glVertex3dv              = void      BSE_OGLAPI( const GLdouble* v );
  using glVertex3f               = void      BSE_OGLAPI( GLfloat x, GLfloat y, GLfloat z );
  using glVertex3fv              = void      BSE_OGLAPI( const GLfloat* v );
  using glVertex3i               = void      BSE_OGLAPI( GLint x, GLint y, GLint z );
  using glVertex3iv              = void      BSE_OGLAPI( const GLint* v );
  using glVertex3s               = void      BSE_OGLAPI( GLshort x, GLshort y, GLshort z );
  using glVertex3sv              = void      BSE_OGLAPI( const GLshort* v );
  using glVertex4d               = void      BSE_OGLAPI( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
  using glVertex4dv              = void      BSE_OGLAPI( const GLdouble* v );
  using glVertex4f               = void      BSE_OGLAPI( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
  using glVertex4fv              = void      BSE_OGLAPI( const GLfloat* v );
  using glVertex4i               = void      BSE_OGLAPI( GLint x, GLint y, GLint z, GLint w );
  using glVertex4iv              = void      BSE_OGLAPI( const GLint* v );
  using glVertex4s               = void      BSE_OGLAPI( GLshort x, GLshort y, GLshort z, GLshort w );
  using glVertex4sv              = void      BSE_OGLAPI( const GLshort* v );
  using glVertexPointer          = void      BSE_OGLAPI( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
  using glViewport               = void      BSE_OGLAPI( GLint x, GLint y, GLsizei width, GLsizei height );

  // OPENGL 2.0 ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  using glShaderSource             = void   BSE_OGLAPI( GLuint shader, GLsizei count, const GLchar** string, const GLint* length );
  using glCreateShader             = GLuint BSE_OGLAPI( GLenum type );
  using glCompileShader            = void   BSE_OGLAPI( GLuint shader );
  using glAttachShader             = void   BSE_OGLAPI( GLuint program, GLuint shader );
  using glDetachShader             = void   BSE_OGLAPI( GLuint program, GLuint shader );
  using glDeleteShader             = void   BSE_OGLAPI( GLuint shader );
  using glCreateProgram            = GLuint BSE_OGLAPI( void );
  using glLinkProgram              = void   BSE_OGLAPI( GLuint program );
  using glDeleteProgram            = void   BSE_OGLAPI( GLuint program );
  using glBufferData               = void   BSE_OGLAPI( GLenum target, GLsizeiptr size, const void* data, GLenum usage );
  using glBufferSubData            = void   BSE_OGLAPI( GLenum target, GLintptr offset, GLsizeiptr size, const void* data );
  using glBindBuffer               = void   BSE_OGLAPI( GLenum target, GLuint buffer );
  using glDeleteBuffers            = void   BSE_OGLAPI( GLsizei n, const GLuint* buffers );
  using glUseProgram               = void   BSE_OGLAPI( GLuint program );
  using glGetProgramiv             = void   BSE_OGLAPI( GLuint program, GLenum pname, GLint* params );
  using glGetProgramInfoLog        = void   BSE_OGLAPI( GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog );
  using glGetShaderiv              = void   BSE_OGLAPI( GLuint shader, GLenum pname, GLint* params );
  using glGetShaderInfoLog         = void   BSE_OGLAPI( GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog );
  using glVertexAttribPointer      = void   BSE_OGLAPI( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer );
  using glDisableVertexAttribArray = void   BSE_OGLAPI( GLuint index );
  using glEnableVertexAttribArray  = void   BSE_OGLAPI( GLuint index );
  using glGenBuffers               = void   BSE_OGLAPI( GLsizei n, GLuint* buffers );
  using glGetActiveAttrib          = void   BSE_OGLAPI( GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name );
  using glGetAttribLocation        = GLint  BSE_OGLAPI( GLuint program, const GLchar* name );
  using glGetActiveUniform         = void   BSE_OGLAPI( GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name );
  using glGetUniformLocation       = GLint  BSE_OGLAPI( GLuint program, const GLchar* name );
  using glGetUniformfv             = void   BSE_OGLAPI( GLuint program, GLint location, GLfloat* params );
  using glGetUniformiv             = void   BSE_OGLAPI( GLuint program, GLint location, GLint* params );
  using glUniform1f                = void   BSE_OGLAPI( GLint location, GLfloat v0 );
  using glUniform2f                = void   BSE_OGLAPI( GLint location, GLfloat v0, GLfloat v1 );
  using glUniform3f                = void   BSE_OGLAPI( GLint location, GLfloat v0, GLfloat v1, GLfloat v2 );
  using glUniform4f                = void   BSE_OGLAPI( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 );
  using glUniform1i                = void   BSE_OGLAPI( GLint location, GLint v0 );
  using glUniform2i                = void   BSE_OGLAPI( GLint location, GLint v0, GLint v1 );
  using glUniform3i                = void   BSE_OGLAPI( GLint location, GLint v0, GLint v1, GLint v2 );
  using glUniform4i                = void   BSE_OGLAPI( GLint location, GLint v0, GLint v1, GLint v2, GLint v3 );
  using glUniform1fv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLfloat* value );
  using glUniform2fv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLfloat* value );
  using glUniform3fv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLfloat* value );
  using glUniform4fv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLfloat* value );
  using glUniform1iv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLint* value );
  using glUniform2iv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLint* value );
  using glUniform3iv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLint* value );
  using glUniform4iv               = void   BSE_OGLAPI( GLint location, GLsizei count, const GLint* value );
  using glUniformMatrix2fv         = void   BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  using glUniformMatrix3fv         = void   BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  using glUniformMatrix4fv         = void   BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );

  // OPENGL 2.1 ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  using glUniformMatrix2x3fv = void BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  using glUniformMatrix3x2fv = void BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  using glUniformMatrix2x4fv = void BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  using glUniformMatrix4x2fv = void BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  using glUniformMatrix3x4fv = void BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
  using glUniformMatrix4x3fv = void BSE_OGLAPI( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );

  // OPENGL 3.0 ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  using glGenVertexArrays    = void BSE_OGLAPI( GLsizei n, GLuint* arrays );
  using glBindVertexArray    = void BSE_OGLAPI( GLuint array );
  using glDeleteVertexArrays = void BSE_OGLAPI( GLsizei n, const GLuint* arrays );
  using glBindBufferBase     = void BSE_OGLAPI( GLenum target, GLuint index, GLuint buffer );
  using glBindBufferRange    = void BSE_OGLAPI( GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size );
  using glUniform1ui         = void BSE_OGLAPI( GLint location, GLuint v0 );
  using glUniform2ui         = void BSE_OGLAPI( GLint location, GLuint v0, GLuint v1 );
  using glUniform3ui         = void BSE_OGLAPI( GLint location, GLuint v0, GLuint v1, GLuint v2 );
  using glUniform4ui         = void BSE_OGLAPI( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 );
  using glUniform1uiv        = void BSE_OGLAPI( GLint location, GLsizei count, const GLuint* value );
  using glUniform2uiv        = void BSE_OGLAPI( GLint location, GLsizei count, const GLuint* value );
  using glUniform3uiv        = void BSE_OGLAPI( GLint location, GLsizei count, const GLuint* value );
  using glUniform4uiv        = void BSE_OGLAPI( GLint location, GLsizei count, const GLuint* value );

  // OPENGL 3.1 ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  using glGetUniformBlockIndex      = GLuint BSE_OGLAPI( GLuint program, const GLchar* uniformBlockName );
  using glUniformBlockBinding       = void   BSE_OGLAPI( GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding );
  using glGetActiveUniformBlockiv   = void   BSE_OGLAPI( GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params );
  using glGetActiveUniformBlockName = void   BSE_OGLAPI( GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName );

  // OPENGL 4.3 ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  using DEBUG_MESSAGE_CALLBACK_SIGNATURE = void BSE_OGLAPI( GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* msg, void const* user_param );
  using glDebugMessageCallback           = void BSE_OGLAPI( DEBUG_MESSAGE_CALLBACK_SIGNATURE callback, const void* userParam );

  // OPENGL 4.5 ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  using glCreateBuffers            = void BSE_OGLAPI( GLsizei n, GLuint* buffers );
  using glNamedBufferData          = void BSE_OGLAPI( GLuint buffer, GLsizeiptr size, const void* data, GLenum usage );
  using glNamedBufferSubData       = void BSE_OGLAPI( GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data );
  using glCreateVertexArrays       = void BSE_OGLAPI( GLsizei n, GLuint* arrays );
  using glEnableVertexArrayAttrib  = void BSE_OGLAPI( GLuint vaobj, GLuint index );
  using glVertexArrayElementBuffer = void BSE_OGLAPI( GLuint vaobj, GLuint buffer );
  using glVertexArrayAttribBinding = void BSE_OGLAPI( GLuint vaobj, GLuint attribindex, GLuint bindingindex );
  using glVertexArrayAttribFormat  = void BSE_OGLAPI( GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset );
  using glVertexArrayVertexBuffer  = void BSE_OGLAPI( GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride );

};

#define BSE_DEFINE_GL(fn) static opengl_ext::fn* fn

BSE_DEFINE_GL( glAccum );
BSE_DEFINE_GL( glAlphaFunc );
BSE_DEFINE_GL( glAreTexturesResident );
BSE_DEFINE_GL( glArrayElement );
BSE_DEFINE_GL( glBegin );
BSE_DEFINE_GL( glBindTexture );
BSE_DEFINE_GL( glBitmap );
BSE_DEFINE_GL( glBlendFunc );
BSE_DEFINE_GL( glCallList );
BSE_DEFINE_GL( glCallLists );
BSE_DEFINE_GL( glClear );
BSE_DEFINE_GL( glClearAccum );
BSE_DEFINE_GL( glClearColor );
BSE_DEFINE_GL( glClearDepth );
BSE_DEFINE_GL( glClearIndex );
BSE_DEFINE_GL( glClearStencil );
BSE_DEFINE_GL( glClipPlane );
BSE_DEFINE_GL( glColor3b );
BSE_DEFINE_GL( glColor3bv );
BSE_DEFINE_GL( glColor3d );
BSE_DEFINE_GL( glColor3dv );
BSE_DEFINE_GL( glColor3f );
BSE_DEFINE_GL( glColor3fv );
BSE_DEFINE_GL( glColor3i );
BSE_DEFINE_GL( glColor3iv );
BSE_DEFINE_GL( glColor3s );
BSE_DEFINE_GL( glColor3sv );
BSE_DEFINE_GL( glColor3ub );
BSE_DEFINE_GL( glColor3ubv );
BSE_DEFINE_GL( glColor3ui );
BSE_DEFINE_GL( glColor3uiv );
BSE_DEFINE_GL( glColor3us );
BSE_DEFINE_GL( glColor3usv );
BSE_DEFINE_GL( glColor4b );
BSE_DEFINE_GL( glColor4bv );
BSE_DEFINE_GL( glColor4d );
BSE_DEFINE_GL( glColor4dv );
BSE_DEFINE_GL( glColor4f );
BSE_DEFINE_GL( glColor4fv );
BSE_DEFINE_GL( glColor4i );
BSE_DEFINE_GL( glColor4iv );
BSE_DEFINE_GL( glColor4s );
BSE_DEFINE_GL( glColor4sv );
BSE_DEFINE_GL( glColor4ub );
BSE_DEFINE_GL( glColor4ubv );
BSE_DEFINE_GL( glColor4ui );
BSE_DEFINE_GL( glColor4uiv );
BSE_DEFINE_GL( glColor4us );
BSE_DEFINE_GL( glColor4usv );
BSE_DEFINE_GL( glColorMask );
BSE_DEFINE_GL( glColorMaterial );
BSE_DEFINE_GL( glColorPointer );
BSE_DEFINE_GL( glCopyPixels );
BSE_DEFINE_GL( glCopyTexImage1D );
BSE_DEFINE_GL( glCopyTexImage2D );
BSE_DEFINE_GL( glCopyTexSubImage1D );
BSE_DEFINE_GL( glCopyTexSubImage2D );
BSE_DEFINE_GL( glCullFace );
BSE_DEFINE_GL( glDeleteLists );
BSE_DEFINE_GL( glDeleteTextures );
BSE_DEFINE_GL( glDepthFunc );
BSE_DEFINE_GL( glDepthMask );
BSE_DEFINE_GL( glDepthRange );
BSE_DEFINE_GL( glDisable );
BSE_DEFINE_GL( glDisableClientState );
BSE_DEFINE_GL( glDrawArrays );
BSE_DEFINE_GL( glDrawBuffer );
BSE_DEFINE_GL( glDrawElements );
BSE_DEFINE_GL( glDrawPixels );
BSE_DEFINE_GL( glEdgeFlag );
BSE_DEFINE_GL( glEdgeFlagPointer );
BSE_DEFINE_GL( glEdgeFlagv );
BSE_DEFINE_GL( glEnable );
BSE_DEFINE_GL( glEnableClientState );
BSE_DEFINE_GL( glEnd );
BSE_DEFINE_GL( glEndList );
BSE_DEFINE_GL( glEvalCoord1d );
BSE_DEFINE_GL( glEvalCoord1dv );
BSE_DEFINE_GL( glEvalCoord1f );
BSE_DEFINE_GL( glEvalCoord1fv );
BSE_DEFINE_GL( glEvalCoord2d );
BSE_DEFINE_GL( glEvalCoord2dv );
BSE_DEFINE_GL( glEvalCoord2f );
BSE_DEFINE_GL( glEvalCoord2fv );
BSE_DEFINE_GL( glEvalMesh1 );
BSE_DEFINE_GL( glEvalMesh2 );
BSE_DEFINE_GL( glEvalPoint1 );
BSE_DEFINE_GL( glEvalPoint2 );
BSE_DEFINE_GL( glFeedbackBuffer );
BSE_DEFINE_GL( glFinish );
BSE_DEFINE_GL( glFlush );
BSE_DEFINE_GL( glFogf );
BSE_DEFINE_GL( glFogfv );
BSE_DEFINE_GL( glFogi );
BSE_DEFINE_GL( glFogiv );
BSE_DEFINE_GL( glFrontFace );
BSE_DEFINE_GL( glFrustum );
BSE_DEFINE_GL( glGenLists );
BSE_DEFINE_GL( glGenTextures );
BSE_DEFINE_GL( glGetBooleanv );
BSE_DEFINE_GL( glGetClipPlane );
BSE_DEFINE_GL( glGetDoublev );
BSE_DEFINE_GL( glGetError );
BSE_DEFINE_GL( glGetFloatv );
BSE_DEFINE_GL( glGetIntegerv );
BSE_DEFINE_GL( glGetLightfv );
BSE_DEFINE_GL( glGetLightiv );
BSE_DEFINE_GL( glGetMapdv );
BSE_DEFINE_GL( glGetMapfv );
BSE_DEFINE_GL( glGetMapiv );
BSE_DEFINE_GL( glGetMaterialfv );
BSE_DEFINE_GL( glGetMaterialiv );
BSE_DEFINE_GL( glGetPixelMapfv );
BSE_DEFINE_GL( glGetPixelMapuiv );
BSE_DEFINE_GL( glGetPixelMapusv );
BSE_DEFINE_GL( glGetPointerv );
BSE_DEFINE_GL( glGetPolygonStipple );
BSE_DEFINE_GL( glGetString );
BSE_DEFINE_GL( glGetTexEnvfv );
BSE_DEFINE_GL( glGetTexEnviv );
BSE_DEFINE_GL( glGetTexGendv );
BSE_DEFINE_GL( glGetTexGenfv );
BSE_DEFINE_GL( glGetTexGeniv );
BSE_DEFINE_GL( glGetTexImage );
BSE_DEFINE_GL( glGetTexLevelParameterfv );
BSE_DEFINE_GL( glGetTexLevelParameteriv );
BSE_DEFINE_GL( glGetTexParameterfv );
BSE_DEFINE_GL( glGetTexParameteriv );
BSE_DEFINE_GL( glHint );
BSE_DEFINE_GL( glIndexMask );
BSE_DEFINE_GL( glIndexPointer );
BSE_DEFINE_GL( glIndexd );
BSE_DEFINE_GL( glIndexdv );
BSE_DEFINE_GL( glIndexf );
BSE_DEFINE_GL( glIndexfv );
BSE_DEFINE_GL( glIndexi );
BSE_DEFINE_GL( glIndexiv );
BSE_DEFINE_GL( glIndexs );
BSE_DEFINE_GL( glIndexsv );
BSE_DEFINE_GL( glIndexub );
BSE_DEFINE_GL( glIndexubv );
BSE_DEFINE_GL( glInitNames );
BSE_DEFINE_GL( glInterleavedArrays );
BSE_DEFINE_GL( glIsEnabled );
BSE_DEFINE_GL( glIsList );
BSE_DEFINE_GL( glIsTexture );
BSE_DEFINE_GL( glLightModelf );
BSE_DEFINE_GL( glLightModelfv );
BSE_DEFINE_GL( glLightModeli );
BSE_DEFINE_GL( glLightModeliv );
BSE_DEFINE_GL( glLightf );
BSE_DEFINE_GL( glLightfv );
BSE_DEFINE_GL( glLighti );
BSE_DEFINE_GL( glLightiv );
BSE_DEFINE_GL( glLineStipple );
BSE_DEFINE_GL( glLineWidth );
BSE_DEFINE_GL( glListBase );
BSE_DEFINE_GL( glLoadIdentity );
BSE_DEFINE_GL( glLoadMatrixd );
BSE_DEFINE_GL( glLoadMatrixf );
BSE_DEFINE_GL( glLoadName );
BSE_DEFINE_GL( glLogicOp );
BSE_DEFINE_GL( glMap1d );
BSE_DEFINE_GL( glMap1f );
BSE_DEFINE_GL( glMap2d );
BSE_DEFINE_GL( glMap2f );
BSE_DEFINE_GL( glMapGrid1d );
BSE_DEFINE_GL( glMapGrid1f );
BSE_DEFINE_GL( glMapGrid2d );
BSE_DEFINE_GL( glMapGrid2f );
BSE_DEFINE_GL( glMaterialf );
BSE_DEFINE_GL( glMaterialfv );
BSE_DEFINE_GL( glMateriali );
BSE_DEFINE_GL( glMaterialiv );
BSE_DEFINE_GL( glMatrixMode );
BSE_DEFINE_GL( glMultMatrixd );
BSE_DEFINE_GL( glMultMatrixf );
BSE_DEFINE_GL( glNewList );
BSE_DEFINE_GL( glNormal3b );
BSE_DEFINE_GL( glNormal3bv );
BSE_DEFINE_GL( glNormal3d );
BSE_DEFINE_GL( glNormal3dv );
BSE_DEFINE_GL( glNormal3f );
BSE_DEFINE_GL( glNormal3fv );
BSE_DEFINE_GL( glNormal3i );
BSE_DEFINE_GL( glNormal3iv );
BSE_DEFINE_GL( glNormal3s );
BSE_DEFINE_GL( glNormal3sv );
BSE_DEFINE_GL( glNormalPointer );
BSE_DEFINE_GL( glOrtho );
BSE_DEFINE_GL( glPassThrough );
BSE_DEFINE_GL( glPixelMapfv );
BSE_DEFINE_GL( glPixelMapuiv );
BSE_DEFINE_GL( glPixelMapusv );
BSE_DEFINE_GL( glPixelStoref );
BSE_DEFINE_GL( glPixelStorei );
BSE_DEFINE_GL( glPixelTransferf );
BSE_DEFINE_GL( glPixelTransferi );
BSE_DEFINE_GL( glPixelZoom );
BSE_DEFINE_GL( glPointSize );
BSE_DEFINE_GL( glPolygonMode );
BSE_DEFINE_GL( glPolygonOffset );
BSE_DEFINE_GL( glPolygonStipple );
BSE_DEFINE_GL( glPopAttrib );
BSE_DEFINE_GL( glPopClientAttrib );
BSE_DEFINE_GL( glPopMatrix );
BSE_DEFINE_GL( glPopName );
BSE_DEFINE_GL( glPrioritizeTextures );
BSE_DEFINE_GL( glPushAttrib );
BSE_DEFINE_GL( glPushClientAttrib );
BSE_DEFINE_GL( glPushMatrix );
BSE_DEFINE_GL( glPushName );
BSE_DEFINE_GL( glRasterPos2d );
BSE_DEFINE_GL( glRasterPos2dv );
BSE_DEFINE_GL( glRasterPos2f );
BSE_DEFINE_GL( glRasterPos2fv );
BSE_DEFINE_GL( glRasterPos2i );
BSE_DEFINE_GL( glRasterPos2iv );
BSE_DEFINE_GL( glRasterPos2s );
BSE_DEFINE_GL( glRasterPos2sv );
BSE_DEFINE_GL( glRasterPos3d );
BSE_DEFINE_GL( glRasterPos3dv );
BSE_DEFINE_GL( glRasterPos3f );
BSE_DEFINE_GL( glRasterPos3fv );
BSE_DEFINE_GL( glRasterPos3i );
BSE_DEFINE_GL( glRasterPos3iv );
BSE_DEFINE_GL( glRasterPos3s );
BSE_DEFINE_GL( glRasterPos3sv );
BSE_DEFINE_GL( glRasterPos4d );
BSE_DEFINE_GL( glRasterPos4dv );
BSE_DEFINE_GL( glRasterPos4f );
BSE_DEFINE_GL( glRasterPos4fv );
BSE_DEFINE_GL( glRasterPos4i );
BSE_DEFINE_GL( glRasterPos4iv );
BSE_DEFINE_GL( glRasterPos4s );
BSE_DEFINE_GL( glRasterPos4sv );
BSE_DEFINE_GL( glReadBuffer );
BSE_DEFINE_GL( glReadPixels );
BSE_DEFINE_GL( glRectd );
BSE_DEFINE_GL( glRectdv );
BSE_DEFINE_GL( glRectf );
BSE_DEFINE_GL( glRectfv );
BSE_DEFINE_GL( glRecti );
BSE_DEFINE_GL( glRectiv );
BSE_DEFINE_GL( glRects );
BSE_DEFINE_GL( glRectsv );
BSE_DEFINE_GL( glRenderMode );
BSE_DEFINE_GL( glRotated );
BSE_DEFINE_GL( glRotatef );
BSE_DEFINE_GL( glScaled );
BSE_DEFINE_GL( glScalef );
BSE_DEFINE_GL( glScissor );
BSE_DEFINE_GL( glSelectBuffer );
BSE_DEFINE_GL( glShadeModel );
BSE_DEFINE_GL( glStencilFunc );
BSE_DEFINE_GL( glStencilMask );
BSE_DEFINE_GL( glStencilOp );
BSE_DEFINE_GL( glTexCoord1d );
BSE_DEFINE_GL( glTexCoord1dv );
BSE_DEFINE_GL( glTexCoord1f );
BSE_DEFINE_GL( glTexCoord1fv );
BSE_DEFINE_GL( glTexCoord1i );
BSE_DEFINE_GL( glTexCoord1iv );
BSE_DEFINE_GL( glTexCoord1s );
BSE_DEFINE_GL( glTexCoord1sv );
BSE_DEFINE_GL( glTexCoord2d );
BSE_DEFINE_GL( glTexCoord2dv );
BSE_DEFINE_GL( glTexCoord2f );
BSE_DEFINE_GL( glTexCoord2fv );
BSE_DEFINE_GL( glTexCoord2i );
BSE_DEFINE_GL( glTexCoord2iv );
BSE_DEFINE_GL( glTexCoord2s );
BSE_DEFINE_GL( glTexCoord2sv );
BSE_DEFINE_GL( glTexCoord3d );
BSE_DEFINE_GL( glTexCoord3dv );
BSE_DEFINE_GL( glTexCoord3f );
BSE_DEFINE_GL( glTexCoord3fv );
BSE_DEFINE_GL( glTexCoord3i );
BSE_DEFINE_GL( glTexCoord3iv );
BSE_DEFINE_GL( glTexCoord3s );
BSE_DEFINE_GL( glTexCoord3sv );
BSE_DEFINE_GL( glTexCoord4d );
BSE_DEFINE_GL( glTexCoord4dv );
BSE_DEFINE_GL( glTexCoord4f );
BSE_DEFINE_GL( glTexCoord4fv );
BSE_DEFINE_GL( glTexCoord4i );
BSE_DEFINE_GL( glTexCoord4iv );
BSE_DEFINE_GL( glTexCoord4s );
BSE_DEFINE_GL( glTexCoord4sv );
BSE_DEFINE_GL( glTexCoordPointer );
BSE_DEFINE_GL( glTexEnvf );
BSE_DEFINE_GL( glTexEnvfv );
BSE_DEFINE_GL( glTexEnvi );
BSE_DEFINE_GL( glTexEnviv );
BSE_DEFINE_GL( glTexGend );
BSE_DEFINE_GL( glTexGendv );
BSE_DEFINE_GL( glTexGenf );
BSE_DEFINE_GL( glTexGenfv );
BSE_DEFINE_GL( glTexGeni );
BSE_DEFINE_GL( glTexGeniv );
BSE_DEFINE_GL( glTexImage1D );
BSE_DEFINE_GL( glTexImage2D );
BSE_DEFINE_GL( glTexParameterf );
BSE_DEFINE_GL( glTexParameterfv );
BSE_DEFINE_GL( glTexParameteri );
BSE_DEFINE_GL( glTexParameteriv );
BSE_DEFINE_GL( glTexSubImage1D );
BSE_DEFINE_GL( glTexSubImage2D );
BSE_DEFINE_GL( glTranslated );
BSE_DEFINE_GL( glTranslatef );
BSE_DEFINE_GL( glVertex2d );
BSE_DEFINE_GL( glVertex2dv );
BSE_DEFINE_GL( glVertex2f );
BSE_DEFINE_GL( glVertex2fv );
BSE_DEFINE_GL( glVertex2i );
BSE_DEFINE_GL( glVertex2iv );
BSE_DEFINE_GL( glVertex2s );
BSE_DEFINE_GL( glVertex2sv );
BSE_DEFINE_GL( glVertex3d );
BSE_DEFINE_GL( glVertex3dv );
BSE_DEFINE_GL( glVertex3f );
BSE_DEFINE_GL( glVertex3fv );
BSE_DEFINE_GL( glVertex3i );
BSE_DEFINE_GL( glVertex3iv );
BSE_DEFINE_GL( glVertex3s );
BSE_DEFINE_GL( glVertex3sv );
BSE_DEFINE_GL( glVertex4d );
BSE_DEFINE_GL( glVertex4dv );
BSE_DEFINE_GL( glVertex4f );
BSE_DEFINE_GL( glVertex4fv );
BSE_DEFINE_GL( glVertex4i );
BSE_DEFINE_GL( glVertex4iv );
BSE_DEFINE_GL( glVertex4s );
BSE_DEFINE_GL( glVertex4sv );
BSE_DEFINE_GL( glVertexPointer );
BSE_DEFINE_GL( glViewport );
BSE_DEFINE_GL( glShaderSource );
BSE_DEFINE_GL( glCreateShader );
BSE_DEFINE_GL( glCompileShader );
BSE_DEFINE_GL( glAttachShader );
BSE_DEFINE_GL( glDetachShader );
BSE_DEFINE_GL( glDeleteShader );
BSE_DEFINE_GL( glCreateProgram );
BSE_DEFINE_GL( glLinkProgram );
BSE_DEFINE_GL( glDeleteProgram );
BSE_DEFINE_GL( glBufferData );
BSE_DEFINE_GL( glBufferSubData );
BSE_DEFINE_GL( glBindBuffer );
BSE_DEFINE_GL( glDeleteBuffers );
BSE_DEFINE_GL( glUseProgram );
BSE_DEFINE_GL( glGetProgramiv );
BSE_DEFINE_GL( glGetProgramInfoLog );
BSE_DEFINE_GL( glGetShaderiv );
BSE_DEFINE_GL( glGetShaderInfoLog );
BSE_DEFINE_GL( glVertexAttribPointer );
BSE_DEFINE_GL( glDisableVertexAttribArray );
BSE_DEFINE_GL( glEnableVertexAttribArray );
BSE_DEFINE_GL( glGenBuffers );
BSE_DEFINE_GL( glGetActiveAttrib );
BSE_DEFINE_GL( glGetAttribLocation );
BSE_DEFINE_GL( glGetActiveUniform );
BSE_DEFINE_GL( glGetUniformLocation );
BSE_DEFINE_GL( glGetUniformfv );
BSE_DEFINE_GL( glGetUniformiv );
BSE_DEFINE_GL( glUniform1f );
BSE_DEFINE_GL( glUniform2f );
BSE_DEFINE_GL( glUniform3f );
BSE_DEFINE_GL( glUniform4f );
BSE_DEFINE_GL( glUniform1i );
BSE_DEFINE_GL( glUniform2i );
BSE_DEFINE_GL( glUniform3i );
BSE_DEFINE_GL( glUniform4i );
BSE_DEFINE_GL( glUniform1ui );
BSE_DEFINE_GL( glUniform2ui );
BSE_DEFINE_GL( glUniform3ui );
BSE_DEFINE_GL( glUniform4ui );
BSE_DEFINE_GL( glUniform1fv );
BSE_DEFINE_GL( glUniform2fv );
BSE_DEFINE_GL( glUniform3fv );
BSE_DEFINE_GL( glUniform4fv );
BSE_DEFINE_GL( glUniform1iv );
BSE_DEFINE_GL( glUniform2iv );
BSE_DEFINE_GL( glUniform3iv );
BSE_DEFINE_GL( glUniform4iv );
BSE_DEFINE_GL( glUniform1uiv );
BSE_DEFINE_GL( glUniform2uiv );
BSE_DEFINE_GL( glUniform3uiv );
BSE_DEFINE_GL( glUniform4uiv );
BSE_DEFINE_GL( glUniformMatrix2fv );
BSE_DEFINE_GL( glUniformMatrix3fv );
BSE_DEFINE_GL( glUniformMatrix4fv );
BSE_DEFINE_GL( glUniformMatrix2x3fv );
BSE_DEFINE_GL( glUniformMatrix3x2fv );
BSE_DEFINE_GL( glUniformMatrix2x4fv );
BSE_DEFINE_GL( glUniformMatrix4x2fv );
BSE_DEFINE_GL( glUniformMatrix3x4fv );
BSE_DEFINE_GL( glUniformMatrix4x3fv );
BSE_DEFINE_GL( glGenVertexArrays );
BSE_DEFINE_GL( glBindVertexArray );
BSE_DEFINE_GL( glDeleteVertexArrays );
BSE_DEFINE_GL( glBindBufferBase );
BSE_DEFINE_GL( glBindBufferRange );
BSE_DEFINE_GL( glGetUniformBlockIndex );
BSE_DEFINE_GL( glUniformBlockBinding );
BSE_DEFINE_GL( glGetActiveUniformBlockiv );
BSE_DEFINE_GL( glGetActiveUniformBlockName );
BSE_DEFINE_GL( glDebugMessageCallback );
BSE_DEFINE_GL( glCreateBuffers );
BSE_DEFINE_GL( glNamedBufferData );
BSE_DEFINE_GL( glNamedBufferSubData );
BSE_DEFINE_GL( glCreateVertexArrays );
BSE_DEFINE_GL( glEnableVertexArrayAttrib );
BSE_DEFINE_GL( glVertexArrayElementBuffer );
BSE_DEFINE_GL( glVertexArrayAttribBinding );
BSE_DEFINE_GL( glVertexArrayAttribFormat );
BSE_DEFINE_GL( glVertexArrayVertexBuffer );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// inl ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void check_gl_error()
{
  GLenum err;
  while ( (err = glGetError()) != GL_NO_ERROR )
  {
    switch ( err )
    {
      case GL_INVALID_ENUM:
      {
        BREAK;
        break;
      }
      case GL_INVALID_VALUE:
      {
        BREAK;
        break;
      }
      case GL_INVALID_OPERATION:
      {
        BREAK;
        break;
      }
      case GL_STACK_OVERFLOW:
      {
        BREAK;
        break;
      }
      case GL_STACK_UNDERFLOW:
      {
        BREAK;
        break;
      }
      case GL_OUT_OF_MEMORY:
      {
        BREAK;
        break;
      }
      default:
      {
        //??
        BREAK;
      }
    }
  }
}

namespace opengl_ext
{
  GLint init( get_proc_address_fn* get_proc_address_function )
  {
    #define BSE_INIT_GL(fn) ::fn = (opengl_ext::fn*) get_proc_address_function(#fn); if (::fn == nullptr) { result = 0; BREAK; }
    GLint result = 1;

    BSE_INIT_GL( glAccum );
    BSE_INIT_GL( glAlphaFunc );
    BSE_INIT_GL( glAreTexturesResident );
    BSE_INIT_GL( glArrayElement );
    BSE_INIT_GL( glBegin );
    BSE_INIT_GL( glBindTexture );
    BSE_INIT_GL( glBitmap );
    BSE_INIT_GL( glBlendFunc );
    BSE_INIT_GL( glCallList );
    BSE_INIT_GL( glCallLists );
    BSE_INIT_GL( glClear );
    BSE_INIT_GL( glClearAccum );
    BSE_INIT_GL( glClearColor );
    BSE_INIT_GL( glClearDepth );
    BSE_INIT_GL( glClearIndex );
    BSE_INIT_GL( glClearStencil );
    BSE_INIT_GL( glClipPlane );
    BSE_INIT_GL( glColor3b );
    BSE_INIT_GL( glColor3bv );
    BSE_INIT_GL( glColor3d );
    BSE_INIT_GL( glColor3dv );
    BSE_INIT_GL( glColor3f );
    BSE_INIT_GL( glColor3fv );
    BSE_INIT_GL( glColor3i );
    BSE_INIT_GL( glColor3iv );
    BSE_INIT_GL( glColor3s );
    BSE_INIT_GL( glColor3sv );
    BSE_INIT_GL( glColor3ub );
    BSE_INIT_GL( glColor3ubv );
    BSE_INIT_GL( glColor3ui );
    BSE_INIT_GL( glColor3uiv );
    BSE_INIT_GL( glColor3us );
    BSE_INIT_GL( glColor3usv );
    BSE_INIT_GL( glColor4b );
    BSE_INIT_GL( glColor4bv );
    BSE_INIT_GL( glColor4d );
    BSE_INIT_GL( glColor4dv );
    BSE_INIT_GL( glColor4f );
    BSE_INIT_GL( glColor4fv );
    BSE_INIT_GL( glColor4i );
    BSE_INIT_GL( glColor4iv );
    BSE_INIT_GL( glColor4s );
    BSE_INIT_GL( glColor4sv );
    BSE_INIT_GL( glColor4ub );
    BSE_INIT_GL( glColor4ubv );
    BSE_INIT_GL( glColor4ui );
    BSE_INIT_GL( glColor4uiv );
    BSE_INIT_GL( glColor4us );
    BSE_INIT_GL( glColor4usv );
    BSE_INIT_GL( glColorMask );
    BSE_INIT_GL( glColorMaterial );
    BSE_INIT_GL( glColorPointer );
    BSE_INIT_GL( glCopyPixels );
    BSE_INIT_GL( glCopyTexImage1D );
    BSE_INIT_GL( glCopyTexImage2D );
    BSE_INIT_GL( glCopyTexSubImage1D );
    BSE_INIT_GL( glCopyTexSubImage2D );
    BSE_INIT_GL( glCullFace );
    BSE_INIT_GL( glDeleteLists );
    BSE_INIT_GL( glDeleteTextures );
    BSE_INIT_GL( glDepthFunc );
    BSE_INIT_GL( glDepthMask );
    BSE_INIT_GL( glDepthRange );
    BSE_INIT_GL( glDisable );
    BSE_INIT_GL( glDisableClientState );
    BSE_INIT_GL( glDrawArrays );
    BSE_INIT_GL( glDrawBuffer );
    BSE_INIT_GL( glDrawElements );
    BSE_INIT_GL( glDrawPixels );
    BSE_INIT_GL( glEdgeFlag );
    BSE_INIT_GL( glEdgeFlagPointer );
    BSE_INIT_GL( glEdgeFlagv );
    BSE_INIT_GL( glEnable );
    BSE_INIT_GL( glEnableClientState );
    BSE_INIT_GL( glEnd );
    BSE_INIT_GL( glEndList );
    BSE_INIT_GL( glEvalCoord1d );
    BSE_INIT_GL( glEvalCoord1dv );
    BSE_INIT_GL( glEvalCoord1f );
    BSE_INIT_GL( glEvalCoord1fv );
    BSE_INIT_GL( glEvalCoord2d );
    BSE_INIT_GL( glEvalCoord2dv );
    BSE_INIT_GL( glEvalCoord2f );
    BSE_INIT_GL( glEvalCoord2fv );
    BSE_INIT_GL( glEvalMesh1 );
    BSE_INIT_GL( glEvalMesh2 );
    BSE_INIT_GL( glEvalPoint1 );
    BSE_INIT_GL( glEvalPoint2 );
    BSE_INIT_GL( glFeedbackBuffer );
    BSE_INIT_GL( glFinish );
    BSE_INIT_GL( glFlush );
    BSE_INIT_GL( glFogf );
    BSE_INIT_GL( glFogfv );
    BSE_INIT_GL( glFogi );
    BSE_INIT_GL( glFogiv );
    BSE_INIT_GL( glFrontFace );
    BSE_INIT_GL( glFrustum );
    BSE_INIT_GL( glGenLists );
    BSE_INIT_GL( glGenTextures );
    BSE_INIT_GL( glGetBooleanv );
    BSE_INIT_GL( glGetClipPlane );
    BSE_INIT_GL( glGetDoublev );
    BSE_INIT_GL( glGetError );
    BSE_INIT_GL( glGetFloatv );
    BSE_INIT_GL( glGetIntegerv );
    BSE_INIT_GL( glGetLightfv );
    BSE_INIT_GL( glGetLightiv );
    BSE_INIT_GL( glGetMapdv );
    BSE_INIT_GL( glGetMapfv );
    BSE_INIT_GL( glGetMapiv );
    BSE_INIT_GL( glGetMaterialfv );
    BSE_INIT_GL( glGetMaterialiv );
    BSE_INIT_GL( glGetPixelMapfv );
    BSE_INIT_GL( glGetPixelMapuiv );
    BSE_INIT_GL( glGetPixelMapusv );
    BSE_INIT_GL( glGetPointerv );
    BSE_INIT_GL( glGetPolygonStipple );
    BSE_INIT_GL( glGetString );
    BSE_INIT_GL( glGetTexEnvfv );
    BSE_INIT_GL( glGetTexEnviv );
    BSE_INIT_GL( glGetTexGendv );
    BSE_INIT_GL( glGetTexGenfv );
    BSE_INIT_GL( glGetTexGeniv );
    BSE_INIT_GL( glGetTexImage );
    BSE_INIT_GL( glGetTexLevelParameterfv );
    BSE_INIT_GL( glGetTexLevelParameteriv );
    BSE_INIT_GL( glGetTexParameterfv );
    BSE_INIT_GL( glGetTexParameteriv );
    BSE_INIT_GL( glHint );
    BSE_INIT_GL( glIndexMask );
    BSE_INIT_GL( glIndexPointer );
    BSE_INIT_GL( glIndexd );
    BSE_INIT_GL( glIndexdv );
    BSE_INIT_GL( glIndexf );
    BSE_INIT_GL( glIndexfv );
    BSE_INIT_GL( glIndexi );
    BSE_INIT_GL( glIndexiv );
    BSE_INIT_GL( glIndexs );
    BSE_INIT_GL( glIndexsv );
    BSE_INIT_GL( glIndexub );
    BSE_INIT_GL( glIndexubv );
    BSE_INIT_GL( glInitNames );
    BSE_INIT_GL( glInterleavedArrays );
    BSE_INIT_GL( glIsEnabled );
    BSE_INIT_GL( glIsList );
    BSE_INIT_GL( glIsTexture );
    BSE_INIT_GL( glLightModelf );
    BSE_INIT_GL( glLightModelfv );
    BSE_INIT_GL( glLightModeli );
    BSE_INIT_GL( glLightModeliv );
    BSE_INIT_GL( glLightf );
    BSE_INIT_GL( glLightfv );
    BSE_INIT_GL( glLighti );
    BSE_INIT_GL( glLightiv );
    BSE_INIT_GL( glLineStipple );
    BSE_INIT_GL( glLineWidth );
    BSE_INIT_GL( glListBase );
    BSE_INIT_GL( glLoadIdentity );
    BSE_INIT_GL( glLoadMatrixd );
    BSE_INIT_GL( glLoadMatrixf );
    BSE_INIT_GL( glLoadName );
    BSE_INIT_GL( glLogicOp );
    BSE_INIT_GL( glMap1d );
    BSE_INIT_GL( glMap1f );
    BSE_INIT_GL( glMap2d );
    BSE_INIT_GL( glMap2f );
    BSE_INIT_GL( glMapGrid1d );
    BSE_INIT_GL( glMapGrid1f );
    BSE_INIT_GL( glMapGrid2d );
    BSE_INIT_GL( glMapGrid2f );
    BSE_INIT_GL( glMaterialf );
    BSE_INIT_GL( glMaterialfv );
    BSE_INIT_GL( glMateriali );
    BSE_INIT_GL( glMaterialiv );
    BSE_INIT_GL( glMatrixMode );
    BSE_INIT_GL( glMultMatrixd );
    BSE_INIT_GL( glMultMatrixf );
    BSE_INIT_GL( glNewList );
    BSE_INIT_GL( glNormal3b );
    BSE_INIT_GL( glNormal3bv );
    BSE_INIT_GL( glNormal3d );
    BSE_INIT_GL( glNormal3dv );
    BSE_INIT_GL( glNormal3f );
    BSE_INIT_GL( glNormal3fv );
    BSE_INIT_GL( glNormal3i );
    BSE_INIT_GL( glNormal3iv );
    BSE_INIT_GL( glNormal3s );
    BSE_INIT_GL( glNormal3sv );
    BSE_INIT_GL( glNormalPointer );
    BSE_INIT_GL( glOrtho );
    BSE_INIT_GL( glPassThrough );
    BSE_INIT_GL( glPixelMapfv );
    BSE_INIT_GL( glPixelMapuiv );
    BSE_INIT_GL( glPixelMapusv );
    BSE_INIT_GL( glPixelStoref );
    BSE_INIT_GL( glPixelStorei );
    BSE_INIT_GL( glPixelTransferf );
    BSE_INIT_GL( glPixelTransferi );
    BSE_INIT_GL( glPixelZoom );
    BSE_INIT_GL( glPointSize );
    BSE_INIT_GL( glPolygonMode );
    BSE_INIT_GL( glPolygonOffset );
    BSE_INIT_GL( glPolygonStipple );
    BSE_INIT_GL( glPopAttrib );
    BSE_INIT_GL( glPopClientAttrib );
    BSE_INIT_GL( glPopMatrix );
    BSE_INIT_GL( glPopName );
    BSE_INIT_GL( glPrioritizeTextures );
    BSE_INIT_GL( glPushAttrib );
    BSE_INIT_GL( glPushClientAttrib );
    BSE_INIT_GL( glPushMatrix );
    BSE_INIT_GL( glPushName );
    BSE_INIT_GL( glRasterPos2d );
    BSE_INIT_GL( glRasterPos2dv );
    BSE_INIT_GL( glRasterPos2f );
    BSE_INIT_GL( glRasterPos2fv );
    BSE_INIT_GL( glRasterPos2i );
    BSE_INIT_GL( glRasterPos2iv );
    BSE_INIT_GL( glRasterPos2s );
    BSE_INIT_GL( glRasterPos2sv );
    BSE_INIT_GL( glRasterPos3d );
    BSE_INIT_GL( glRasterPos3dv );
    BSE_INIT_GL( glRasterPos3f );
    BSE_INIT_GL( glRasterPos3fv );
    BSE_INIT_GL( glRasterPos3i );
    BSE_INIT_GL( glRasterPos3iv );
    BSE_INIT_GL( glRasterPos3s );
    BSE_INIT_GL( glRasterPos3sv );
    BSE_INIT_GL( glRasterPos4d );
    BSE_INIT_GL( glRasterPos4dv );
    BSE_INIT_GL( glRasterPos4f );
    BSE_INIT_GL( glRasterPos4fv );
    BSE_INIT_GL( glRasterPos4i );
    BSE_INIT_GL( glRasterPos4iv );
    BSE_INIT_GL( glRasterPos4s );
    BSE_INIT_GL( glRasterPos4sv );
    BSE_INIT_GL( glReadBuffer );
    BSE_INIT_GL( glReadPixels );
    BSE_INIT_GL( glRectd );
    BSE_INIT_GL( glRectdv );
    BSE_INIT_GL( glRectf );
    BSE_INIT_GL( glRectfv );
    BSE_INIT_GL( glRecti );
    BSE_INIT_GL( glRectiv );
    BSE_INIT_GL( glRects );
    BSE_INIT_GL( glRectsv );
    BSE_INIT_GL( glRenderMode );
    BSE_INIT_GL( glRotated );
    BSE_INIT_GL( glRotatef );
    BSE_INIT_GL( glScaled );
    BSE_INIT_GL( glScalef );
    BSE_INIT_GL( glScissor );
    BSE_INIT_GL( glSelectBuffer );
    BSE_INIT_GL( glShadeModel );
    BSE_INIT_GL( glStencilFunc );
    BSE_INIT_GL( glStencilMask );
    BSE_INIT_GL( glStencilOp );
    BSE_INIT_GL( glTexCoord1d );
    BSE_INIT_GL( glTexCoord1dv );
    BSE_INIT_GL( glTexCoord1f );
    BSE_INIT_GL( glTexCoord1fv );
    BSE_INIT_GL( glTexCoord1i );
    BSE_INIT_GL( glTexCoord1iv );
    BSE_INIT_GL( glTexCoord1s );
    BSE_INIT_GL( glTexCoord1sv );
    BSE_INIT_GL( glTexCoord2d );
    BSE_INIT_GL( glTexCoord2dv );
    BSE_INIT_GL( glTexCoord2f );
    BSE_INIT_GL( glTexCoord2fv );
    BSE_INIT_GL( glTexCoord2i );
    BSE_INIT_GL( glTexCoord2iv );
    BSE_INIT_GL( glTexCoord2s );
    BSE_INIT_GL( glTexCoord2sv );
    BSE_INIT_GL( glTexCoord3d );
    BSE_INIT_GL( glTexCoord3dv );
    BSE_INIT_GL( glTexCoord3f );
    BSE_INIT_GL( glTexCoord3fv );
    BSE_INIT_GL( glTexCoord3i );
    BSE_INIT_GL( glTexCoord3iv );
    BSE_INIT_GL( glTexCoord3s );
    BSE_INIT_GL( glTexCoord3sv );
    BSE_INIT_GL( glTexCoord4d );
    BSE_INIT_GL( glTexCoord4dv );
    BSE_INIT_GL( glTexCoord4f );
    BSE_INIT_GL( glTexCoord4fv );
    BSE_INIT_GL( glTexCoord4i );
    BSE_INIT_GL( glTexCoord4iv );
    BSE_INIT_GL( glTexCoord4s );
    BSE_INIT_GL( glTexCoord4sv );
    BSE_INIT_GL( glTexCoordPointer );
    BSE_INIT_GL( glTexEnvf );
    BSE_INIT_GL( glTexEnvfv );
    BSE_INIT_GL( glTexEnvi );
    BSE_INIT_GL( glTexEnviv );
    BSE_INIT_GL( glTexGend );
    BSE_INIT_GL( glTexGendv );
    BSE_INIT_GL( glTexGenf );
    BSE_INIT_GL( glTexGenfv );
    BSE_INIT_GL( glTexGeni );
    BSE_INIT_GL( glTexGeniv );
    BSE_INIT_GL( glTexImage1D );
    BSE_INIT_GL( glTexImage2D );
    BSE_INIT_GL( glTexParameterf );
    BSE_INIT_GL( glTexParameterfv );
    BSE_INIT_GL( glTexParameteri );
    BSE_INIT_GL( glTexParameteriv );
    BSE_INIT_GL( glTexSubImage1D );
    BSE_INIT_GL( glTexSubImage2D );
    BSE_INIT_GL( glTranslated );
    BSE_INIT_GL( glTranslatef );
    BSE_INIT_GL( glVertex2d );
    BSE_INIT_GL( glVertex2dv );
    BSE_INIT_GL( glVertex2f );
    BSE_INIT_GL( glVertex2fv );
    BSE_INIT_GL( glVertex2i );
    BSE_INIT_GL( glVertex2iv );
    BSE_INIT_GL( glVertex2s );
    BSE_INIT_GL( glVertex2sv );
    BSE_INIT_GL( glVertex3d );
    BSE_INIT_GL( glVertex3dv );
    BSE_INIT_GL( glVertex3f );
    BSE_INIT_GL( glVertex3fv );
    BSE_INIT_GL( glVertex3i );
    BSE_INIT_GL( glVertex3iv );
    BSE_INIT_GL( glVertex3s );
    BSE_INIT_GL( glVertex3sv );
    BSE_INIT_GL( glVertex4d );
    BSE_INIT_GL( glVertex4dv );
    BSE_INIT_GL( glVertex4f );
    BSE_INIT_GL( glVertex4fv );
    BSE_INIT_GL( glVertex4i );
    BSE_INIT_GL( glVertex4iv );
    BSE_INIT_GL( glVertex4s );
    BSE_INIT_GL( glVertex4sv );
    BSE_INIT_GL( glVertexPointer );
    BSE_INIT_GL( glViewport );
    BSE_INIT_GL( glShaderSource );
    BSE_INIT_GL( glCreateShader );
    BSE_INIT_GL( glCompileShader );
    BSE_INIT_GL( glAttachShader );
    BSE_INIT_GL( glDetachShader );
    BSE_INIT_GL( glDeleteShader );
    BSE_INIT_GL( glCreateProgram );
    BSE_INIT_GL( glLinkProgram );
    BSE_INIT_GL( glDeleteProgram );
    BSE_INIT_GL( glBufferData );
    BSE_INIT_GL( glBufferSubData );
    BSE_INIT_GL( glBindBuffer );
    BSE_INIT_GL( glDeleteBuffers );
    BSE_INIT_GL( glUseProgram );
    BSE_INIT_GL( glGetProgramiv );
    BSE_INIT_GL( glGetProgramInfoLog );
    BSE_INIT_GL( glGetShaderiv );
    BSE_INIT_GL( glGetShaderInfoLog );
    BSE_INIT_GL( glVertexAttribPointer );
    BSE_INIT_GL( glDisableVertexAttribArray );
    BSE_INIT_GL( glEnableVertexAttribArray );
    BSE_INIT_GL( glGenBuffers );
    BSE_INIT_GL( glGetActiveAttrib );
    BSE_INIT_GL( glGetAttribLocation );
    BSE_INIT_GL( glGetActiveUniform );
    BSE_INIT_GL( glGetUniformLocation );
    BSE_INIT_GL( glGetUniformfv );
    BSE_INIT_GL( glGetUniformiv );
    BSE_INIT_GL( glUniform1f );
    BSE_INIT_GL( glUniform2f );
    BSE_INIT_GL( glUniform3f );
    BSE_INIT_GL( glUniform4f );
    BSE_INIT_GL( glUniform1i );
    BSE_INIT_GL( glUniform2i );
    BSE_INIT_GL( glUniform3i );
    BSE_INIT_GL( glUniform4i );
    BSE_INIT_GL( glUniform1ui );
    BSE_INIT_GL( glUniform2ui );
    BSE_INIT_GL( glUniform3ui );
    BSE_INIT_GL( glUniform4ui );
    BSE_INIT_GL( glUniform1fv );
    BSE_INIT_GL( glUniform2fv );
    BSE_INIT_GL( glUniform3fv );
    BSE_INIT_GL( glUniform4fv );
    BSE_INIT_GL( glUniform1iv );
    BSE_INIT_GL( glUniform2iv );
    BSE_INIT_GL( glUniform3iv );
    BSE_INIT_GL( glUniform4iv );
    BSE_INIT_GL( glUniform1uiv );
    BSE_INIT_GL( glUniform2uiv );
    BSE_INIT_GL( glUniform3uiv );
    BSE_INIT_GL( glUniform4uiv );
    BSE_INIT_GL( glUniformMatrix2fv );
    BSE_INIT_GL( glUniformMatrix3fv );
    BSE_INIT_GL( glUniformMatrix4fv );
    BSE_INIT_GL( glUniformMatrix2x3fv );
    BSE_INIT_GL( glUniformMatrix3x2fv );
    BSE_INIT_GL( glUniformMatrix2x4fv );
    BSE_INIT_GL( glUniformMatrix4x2fv );
    BSE_INIT_GL( glUniformMatrix3x4fv );
    BSE_INIT_GL( glUniformMatrix4x3fv );
    BSE_INIT_GL( glGenVertexArrays );
    BSE_INIT_GL( glBindVertexArray );
    BSE_INIT_GL( glDeleteVertexArrays );
    BSE_INIT_GL( glBindBufferBase );
    BSE_INIT_GL( glBindBufferRange );
    BSE_INIT_GL( glGetUniformBlockIndex );
    BSE_INIT_GL( glUniformBlockBinding );
    BSE_INIT_GL( glGetActiveUniformBlockiv );
    BSE_INIT_GL( glGetActiveUniformBlockName );
    BSE_INIT_GL( glCreateBuffers );
    BSE_INIT_GL( glDebugMessageCallback );
    BSE_INIT_GL( glNamedBufferData );
    BSE_INIT_GL( glNamedBufferSubData );
    BSE_INIT_GL( glCreateVertexArrays );
    BSE_INIT_GL( glEnableVertexArrayAttrib );
    BSE_INIT_GL( glVertexArrayElementBuffer );
    BSE_INIT_GL( glVertexArrayAttribBinding );
    BSE_INIT_GL( glVertexArrayAttribFormat );
    BSE_INIT_GL( glVertexArrayVertexBuffer );

    return result;
  }
};