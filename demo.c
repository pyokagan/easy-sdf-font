/*
 * gcc $(pkg-config --cflags sdl2) demo.c $(pkg-config --libs sdl2) -lm
 */
#include <SDL.h>
#include <SDL_opengles2.h>

#define ESFONT_STATIC
#define EASY_SDF_FONT_IMPLEMENTATION
#include "easy_sdf_font.h"

static const char vertexShader[] =
    "uniform mat4 m;\n"
    "attribute vec2 pos, uv;\n"
    "varying vec2 vuv;\n"
    "void main() {\n"
    "    gl_Position = m * vec4(pos, 0.0, 1.0);\n"
    "    vuv = uv;\n"
    "}\n";

static const char fragmentShader[] =
    "precision mediump float;\n"
    "varying vec2 vuv;\n"
    "uniform sampler2D tex;\n"
    "void main() {\n"
    "    float d = texture2D(tex, vuv).r;\n"
    "    float smooth_factor = 0.06;\n"
    "    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0) * smoothstep(0.5 - smooth_factor, 0.5 + smooth_factor, d);\n"
    "}\n";

static struct {
    void (GL_APIENTRYP genTextures)(GLsizei, GLuint *);
    void (GL_APIENTRYP bindTexture)(GLenum, GLuint);
    void (GL_APIENTRYP texImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
    void (GL_APIENTRYP generateMipmap)(GLenum);
    GLuint (GL_APIENTRYP createShader)(GLenum);
    void (GL_APIENTRYP shaderSource)(GLuint, GLsizei, const GLchar* const* string, const GLint*);
    void (GL_APIENTRYP compileShader)(GLuint);
    GLuint (GL_APIENTRYP createProgram)(void);
    void (GL_APIENTRYP attachShader)(GLuint, GLuint);
    void (GL_APIENTRYP bindAttribLocation)(GLuint, GLuint, const GLchar*);
    void (GL_APIENTRYP linkProgram)(GLuint);
    void (GL_APIENTRYP genBuffers)(GLsizei, GLuint *);
    GLint (GL_APIENTRYP getUniformLocation)(GLuint, const GLchar *);
    void (GL_APIENTRYP viewport)(GLint, GLint, GLsizei, GLsizei);
    void (GL_APIENTRYP clearColor)(GLclampf, GLclampf, GLclampf, GLclampf);
    void (GL_APIENTRYP clear)(GLbitfield);
    void (GL_APIENTRYP enable)(GLenum);
    void (GL_APIENTRYP blendFunc)(GLenum, GLenum);
    void (GL_APIENTRYP useProgram)(GLuint);
    void (GL_APIENTRYP uniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat *);
    void (GL_APIENTRYP bindBuffer)(GLenum, GLuint);
    void (GL_APIENTRYP bufferData)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
    void (GL_APIENTRYP vertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
    void (GL_APIENTRYP enableVertexAttribArray)(GLuint);
    void (GL_APIENTRYP drawArrays)(GLenum, GLint, GLsizei);
    void (GL_APIENTRYP getShaderiv)(GLuint, GLenum, GLint *);
    void (GL_APIENTRYP getProgramiv)(GLuint, GLenum, GLint *);
} gl;

int
main(int argc, char *argv[])
{
    float scale = 2.0f;
    int padding = 2;
    float thickness = 0.5f * scale;
    unsigned char onedge_value = 128;
    float pixel_dist_scale = (float)onedge_value / padding;
    int textureSize = 512;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_Window *sdlWindow = SDL_CreateWindow("easy-sdf-font", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 300, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!sdlWindow) {
        SDL_Log("failed to create window: %s", SDL_GetError());
        return 1;
    }

    SDL_GLContext glctx = SDL_GL_CreateContext(sdlWindow);
    if (!glctx) {
        SDL_Log("failed to create opengl context: %s", SDL_GetError());
        return 1;
    }

    gl.genTextures = SDL_GL_GetProcAddress("glGenTextures");
    gl.bindTexture = SDL_GL_GetProcAddress("glBindTexture");
    gl.texImage2D = SDL_GL_GetProcAddress("glTexImage2D");
    gl.generateMipmap = SDL_GL_GetProcAddress("glGenerateMipmap");
    gl.createShader = SDL_GL_GetProcAddress("glCreateShader");
    gl.shaderSource = SDL_GL_GetProcAddress("glShaderSource");
    gl.compileShader = SDL_GL_GetProcAddress("glCompileShader");
    gl.createProgram = SDL_GL_GetProcAddress("glCreateProgram");
    gl.attachShader = SDL_GL_GetProcAddress("glAttachShader");
    gl.bindAttribLocation = SDL_GL_GetProcAddress("glBindAttribLocation");
    gl.linkProgram = SDL_GL_GetProcAddress("glLinkProgram");
    gl.genBuffers = SDL_GL_GetProcAddress("glGenBuffers");
    gl.getUniformLocation = SDL_GL_GetProcAddress("glGetUniformLocation");
    gl.viewport = SDL_GL_GetProcAddress("glViewport");
    gl.clearColor = SDL_GL_GetProcAddress("glClearColor");
    gl.clear = SDL_GL_GetProcAddress("glClear");
    gl.enable = SDL_GL_GetProcAddress("glEnable");
    gl.blendFunc = SDL_GL_GetProcAddress("glBlendFunc");
    gl.useProgram = SDL_GL_GetProcAddress("glUseProgram");
    gl.uniformMatrix4fv = SDL_GL_GetProcAddress("glUniformMatrix4fv");
    gl.bindBuffer = SDL_GL_GetProcAddress("glBindBuffer");
    gl.bufferData = SDL_GL_GetProcAddress("glBufferData");
    gl.vertexAttribPointer = SDL_GL_GetProcAddress("glVertexAttribPointer");
    gl.enableVertexAttribArray = SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    gl.drawArrays = SDL_GL_GetProcAddress("glDrawArrays");
    gl.getShaderiv = SDL_GL_GetProcAddress("glGetShaderiv");
    gl.getProgramiv = SDL_GL_GetProcAddress("glGetProgramiv");

    unsigned char *pixels = SDL_malloc(textureSize*textureSize);
    struct esfont_glyph_pack packInfo;
    Uint32 t1 = SDL_GetTicks();
    if (!esfont_PackGlyphs(pixels, textureSize, textureSize, scale, padding, onedge_value, pixel_dist_scale, thickness, &packInfo)) {
        SDL_Log("texture too small");
        return 1;
    }
    Uint32 t2 = SDL_GetTicks();
    char timeMsg[40];
    SDL_snprintf(timeMsg, sizeof(timeMsg), "Texture generation took %u ms.", t2 - t1);

    GLuint tex = 0;
    gl.genTextures(1, &tex);
    gl.bindTexture(GL_TEXTURE_2D, tex);
    gl.texImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, textureSize, textureSize, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
    gl.generateMipmap(GL_TEXTURE_2D);
    int compiled;
    GLuint s1 = gl.createShader(GL_VERTEX_SHADER);
    const GLchar *s1_s[] = { vertexShader };
    gl.shaderSource(s1, 1, s1_s, NULL);
    gl.compileShader(s1);
    gl.getShaderiv(s1, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        SDL_Log("failed to compile shader");
        return 1;
    }
    GLuint s2 = gl.createShader(GL_FRAGMENT_SHADER);
    const GLchar *s2_s[] = { fragmentShader };
    gl.shaderSource(s2, 1, s2_s, NULL);
    gl.compileShader(s2);
    gl.getShaderiv(s2, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        SDL_Log("failed to compile shader");
        return 1;
    }
    GLuint prog = gl.createProgram();
    gl.attachShader(prog, s1);
    gl.attachShader(prog, s2);
    gl.bindAttribLocation(prog, 0, "pos");
    gl.bindAttribLocation(prog, 1, "uv");
    gl.linkProgram(prog);
    gl.getProgramiv(prog, GL_LINK_STATUS, &compiled);
    if (compiled == 0) {
        SDL_Log("failed to link shader");
        return 1;
    }

    struct esfont_tri2 verts[200];
    GLuint vertsBuf = 0;
    gl.genBuffers(1, &vertsBuf);

    GLint matLoc = gl.getUniformLocation(prog, "m");

    for (;;) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                return 0;
            }
        }

        int windowW, windowH;
        SDL_GetWindowSize(sdlWindow, &windowW, &windowH);

        int count = 0;
        count += esfont_GetTextTri2(&packInfo, "Easy SDF Font", 10, 60, 2.0, verts + count, SDL_arraysize(verts) - count);
        count += esfont_GetTextTri2(&packInfo, "The quick brown fox jumped over the lazy dog.", 10, 120, 1, verts + count, SDL_arraysize(verts) - count);
        count += esfont_GetTextTri2(&packInfo, "!\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~", 10, 160, 1, verts + count, SDL_arraysize(verts) - count);
        count += esfont_GetTextTri2(&packInfo, timeMsg, 10, 200, 1, verts + count, SDL_arraysize(verts) - count);
        gl.viewport(0, 0, windowW, windowH);
        gl.clearColor(1, 1, 1, 1);
        gl.clear(GL_COLOR_BUFFER_BIT);
        gl.enable(GL_BLEND);
        gl.blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gl.useProgram(prog);
        // orthographic projection
        float mat[16];
        mat[0] = 2.0f / windowW;
        mat[1] = 0.0f;
        mat[2] = 0.0f;
        mat[3] = 0.0f;
        mat[4] = 0.0f;
        mat[5] = -2.0f / windowH;
        mat[6] = 0.0f;
        mat[7] = 0.0f;
        mat[8] = 0.0f;
        mat[9] = 0.0f;
        mat[10] = 1.0f;
        mat[11] = 0.0f;
        mat[12] = -1.0f;
        mat[13] = 1.0f;
        mat[14] = 0.0f;
        mat[15] = 1.0f;
        gl.uniformMatrix4fv(matLoc, 1, GL_FALSE, mat);
        gl.bindBuffer(GL_ARRAY_BUFFER, vertsBuf);
        gl.bufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        gl.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void *)0);
        gl.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void *)8);
        gl.enableVertexAttribArray(0);
        gl.enableVertexAttribArray(1);
        gl.drawArrays(GL_TRIANGLES, 0, count * 6);
        SDL_GL_SwapWindow(sdlWindow);
    }

    return 0;
}
