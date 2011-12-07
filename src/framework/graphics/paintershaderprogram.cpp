/*
 * Copyright (c) 2010-2011 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "paintershaderprogram.h"
#include "painter.h"
#include "texture.h"
#include "texturemanager.h"

void PainterShaderProgram::setProjectionMatrix(float projectionMatrix[3][3])
{
    setUniformValue(PROJECTION_MATRIX_UNIFORM, projectionMatrix, true);
}

void PainterShaderProgram::setColor(const Color& color)
{
    setUniformValue(COLOR_UNIFORM,
                    color.r() / 255.0f,
                    color.g() / 255.0f,
                    color.b() / 255.0f,
                    color.a() / 255.0f);
}

void PainterShaderProgram::setOpacity(GLfloat opacity)
{
    setUniformValue(OPACITY_UNIFORM, opacity);
}

void PainterShaderProgram::setTexture(const TexturePtr& texture)
{
    float w = texture->getGlSize().width();
    float h = texture->getGlSize().height();

    GLfloat textureTransformMatrix[2][2] = {
        { 1.0f/w,  0.0f   },
        { 0.0f,    1.0f/h }
    };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getId());
    setUniformValue(TEXTURE_UNIFORM, 0);
    setUniformValue(TEXTURE_TRANSFORM_MATRIX_UNIFORM, textureTransformMatrix, true);
}

void PainterShaderProgram::setVertexCoords(const GLfloat *vertices)
{
    enableAttributeArray(PainterShaderProgram::VERTEX_COORDS_ATTR);
    setAttributeArray(PainterShaderProgram::VERTEX_COORDS_ATTR, vertices, 2);
}

void PainterShaderProgram::setTextureCoords(const GLfloat *textureCoords)
{
    enableAttributeArray(PainterShaderProgram::TEXTURE_COORDS_ATTR);
    setAttributeArray(PainterShaderProgram::TEXTURE_COORDS_ATTR, textureCoords, 2);
    m_mustDisableTexCoordsArray = true;
}

void PainterShaderProgram::prepareForDraw()
{
    assert(bind());
}

void PainterShaderProgram::drawTriangleStrip(int numVertices)
{
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
}

void PainterShaderProgram::releaseFromDraw()
{
    if(m_mustDisableTexCoordsArray) {
        disableAttributeArray(PainterShaderProgram::TEXTURE_COORDS_ATTR);
        m_mustDisableTexCoordsArray = false;
    }

    if(m_mustDisableVertexArray) {
        disableAttributeArray(PainterShaderProgram::VERTEX_COORDS_ATTR);
        m_mustDisableVertexArray = false;
    }

    //release();
}
