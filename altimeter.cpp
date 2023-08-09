#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint program;

GLint positionLocFrame;
GLint positionLocIndicator;
GLint positionLocVisibility;
GLint colorLocFrame;
GLint colorLocIndicator;
GLint colorLocVisibility;

GLint texCoordLoc;
GLint textureLoc;

float altitudeValue = 0.0f;

const char* vertexShaderSource =
    "attribute vec2 position;\n"
    "attribute vec2 texCoord;\n"
    "varying vec2 v_texCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "    v_texCoord = texCoord;\n"
    "}\n";
const char* fragmentShaderSource =
    "precision mediump float;\n"
    "varying vec2 v_texCoord;\n"
    "uniform sampler2D texture;\n"
    "uniform vec4 color;\n"
    "void main()\n"
    "{\n"
    "    vec4 fragColor;\n"
    "    if (v_texCoord.y > 0.0 && v_texCoord.x > 0.0) {\n"
    "        vec4 texColor = texture2D(texture, v_texCoord);\n"
    "        fragColor = vec4(texColor.rgb, 1.0);\n"
    "    }\n"
    "    else {\n"
    "        fragColor = color;\n"
    "    }\n"
    "    gl_FragColor = fragColor;\n"
    "}\n";


void createProgram()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glBindAttribLocation(program, 0, "position");

    glLinkProgram(program);

    positionLocFrame = glGetAttribLocation(program, "position");
    positionLocIndicator = glGetAttribLocation(program, "position");
    positionLocVisibility = glGetAttribLocation(program,"position");
    colorLocFrame = glGetUniformLocation(program, "color");
    colorLocIndicator = glGetUniformLocation(program, "color");
    colorLocVisibility = glGetUniformLocation(program,"color");

    texCoordLoc = glGetAttribLocation(program, "texCoord");
    textureLoc = glGetUniformLocation(program, "texture");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
}

GLuint createTexture(const char* imagePath)
{
  
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(imagePath, &width, &height, &channels, 0);

    if (!image) {
        std::cerr << "Failed to load texture image: " << imagePath << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format;
    if (channels == 3) {
        format = GL_RGB; // 3 renk kanalı (RGB)
    }
    else if (channels == 4) {
        format = GL_RGBA; // 4 renk kanalı (RGBA)
    }
    else {
        // Geçersiz resim formatı durumunda hata mesajı verip textureID'yi serbest bırakıyoruz.
        std::cerr << "Invalid texture format: " << imagePath << std::endl;
        stbi_image_free(image);
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);


    return textureID;
}

void drawFrame()
{
    const float radius = 1.0f;
    const float centerX = 0.0f;
    const float centerY = 0.0f;
    const int numSegments = 200;
    const float angleStep = (2.0f * M_PI) / numSegments;

    GLfloat frameVertices[(numSegments + 1) * 2];
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);
        frameVertices[i * 2] = x;
        frameVertices[i * 2 + 1] = y;
    }
    glUniform4f(colorLocFrame, 1.0f, 1.0f, 1.0f, 1.0f);
    glVertexAttribPointer(positionLocFrame, 2, GL_FLOAT, GL_FALSE, 0, frameVertices);
    glEnableVertexAttribArray(positionLocFrame);
    glDrawArrays(GL_LINE_LOOP, 0, numSegments);
}

void drawTexturedCircle(GLuint textureID)
{
    const float radius = 1.0f;
    const float centerX = 0.0f;
    const float centerY = 0.0f;
    const int numSegments = 100;
    const float angleStep = (2.0f * M_PI) / numSegments;

    GLfloat circleVertices[(numSegments + 1) * 2];
    GLfloat texCoords[(numSegments + 1) * 2];

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);
        circleVertices[i * 2] = x;
        circleVertices[i * 2 + 1] = y;

        float s = (x + radius) / (2 * radius);
        float t = (y + radius) / (2 * radius);
        texCoords[i * 2] = s;
        texCoords[i * 2 + 1] = t;
    }

    glVertexAttribPointer(positionLocFrame, 2, GL_FLOAT, GL_FALSE, 0, circleVertices);
    glEnableVertexAttribArray(positionLocFrame);

    glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray(texCoordLoc);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureLoc, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);
}

void drawRedNeedle(float rotationAngle)
{
    const float centerX = 0.0f;
    const float centerY = 0.0f;
    const float centerWidth = 0.012f;
    const float frontHeight = 0.755f;
    const float tipHeight = 0.045;

    float rotationMatrix[4] = {
        cosf(rotationAngle), -sinf(rotationAngle),
        sinf(rotationAngle), cosf(rotationAngle)
    };

    GLfloat indicatorVertices[] = {
        centerX - centerWidth / 2, centerY,
        centerX + centerWidth / 2, centerY,
        centerX - centerWidth / 2 , centerY + frontHeight,
        centerX - centerWidth / 2 , centerY + frontHeight,
        centerX + centerWidth / 2 , centerY + frontHeight,
        centerX + centerWidth / 2 , centerY,
        centerX - centerWidth / 2, centerY + frontHeight,
        centerX - centerWidth * 3.8, centerY + frontHeight + tipHeight,
        centerX + centerWidth / 2, centerY + frontHeight ,
        centerX - centerWidth * 3.8, centerY + frontHeight + tipHeight,
        centerX + centerWidth * 3.8, centerY + frontHeight + tipHeight,
        centerX + centerWidth / 2, centerY + frontHeight
    };

    for (int i = 0; i < 12; ++i) {
        float x = indicatorVertices[i * 2];
        float y = indicatorVertices[i * 2 + 1];
        indicatorVertices[i * 2] = x * rotationMatrix[0] + y * rotationMatrix[2];
        indicatorVertices[i * 2 + 1] = x * rotationMatrix[1] + y * rotationMatrix[3];
    }

    glUniform4f(colorLocIndicator, 1.0f, 0.1f, 0.1f, 1.0f);
    glVertexAttribPointer(positionLocIndicator, 2, GL_FLOAT, GL_FALSE, 0, indicatorVertices);
    glEnableVertexAttribArray(positionLocIndicator);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

void drawShortNeedle(float rotationAngle)
{
    const float centerX = 0.0f;
    const float centerY = 0.0f;
    const float centerWidth = 0.06f;
    const float innerCenterWidth = 0.05f;
    const float frontHeight = 0.4f;
    const float innerfrontHeight = 0.05f;
    const float backHeight = 0.1f;
    const float innerBackHeight = 0.09f;

    float rotationMatrix[4] = {
        cosf(rotationAngle), -sinf(rotationAngle),
        sinf(rotationAngle), cosf(rotationAngle)
    };

    GLfloat indicatorFrontVertices[] = {
        centerX, centerY + frontHeight,
        centerX - centerWidth*0.8, centerY + frontHeight - 0.1f,
        centerX + centerWidth*0.8, centerY + frontHeight - 0.1f,

        centerX - centerWidth*0.8, centerY + frontHeight - 0.1f,
        centerX + centerWidth*0.8, centerY + frontHeight - 0.1f,
        centerX + centerWidth / 2, centerY,

        centerX - centerWidth*0.8, centerY + frontHeight - 0.1f,
        centerX + centerWidth / 2, centerY,
        centerX - centerWidth / 2, centerY,

        centerX + centerWidth / 2, centerY,
        centerX - centerWidth / 2, centerY,
        centerX - centerWidth / 1.3, centerY - backHeight,

        centerX + centerWidth / 2, centerY,
        centerX - centerWidth / 1.3, centerY - backHeight,
        centerX + centerWidth / 1.3, centerY - backHeight,

        centerX - centerWidth / 1.3, centerY - backHeight,
        centerX + centerWidth / 1.3, centerY - backHeight,
        centerX, centerY - backHeight - 0.02f

    };
    for (int i = 0; i < 18; ++i) {
        float x = indicatorFrontVertices[i * 2];
        float y = indicatorFrontVertices[i * 2 + 1];
        indicatorFrontVertices[i * 2] = x * rotationMatrix[0] + y * rotationMatrix[2];
        indicatorFrontVertices[i * 2 + 1] = x * rotationMatrix[1] + y * rotationMatrix[3];
        
    };

    glUniform4f(colorLocIndicator, 0.9f, 0.9f, 0.9f, 1.0f);
    glVertexAttribPointer(positionLocIndicator, 2, GL_FLOAT, GL_FALSE, 0, indicatorFrontVertices);
    glEnableVertexAttribArray(positionLocIndicator);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    GLfloat indicatorBackVertices[] = {

        centerX + innerCenterWidth / 2, centerY,
        centerX - innerCenterWidth / 2, centerY,
        centerX - innerCenterWidth / 1.3, centerY - innerBackHeight,

        centerX + innerCenterWidth / 2, centerY,
        centerX - innerCenterWidth / 1.3, centerY - innerBackHeight,
        centerX + innerCenterWidth / 1.3, centerY - innerBackHeight,

        centerX - innerCenterWidth / 1.3, centerY - innerBackHeight,
        centerX + innerCenterWidth / 1.3, centerY - innerBackHeight,
        centerX, centerY - backHeight - 0.01f
    };
    for (int i = 0; i < 9; ++i) {
        float x = indicatorBackVertices[i * 2];
        float y = indicatorBackVertices[i * 2 + 1];
        indicatorBackVertices[i * 2] = x * rotationMatrix[0] + y * rotationMatrix[2];
        indicatorBackVertices[i * 2 + 1] = x * rotationMatrix[1] + y * rotationMatrix[3];
        
    };

    glUniform4f(colorLocIndicator, 0.0f, 0.0f, 0.0f, 1.0f);
    glVertexAttribPointer(positionLocIndicator, 2, GL_FLOAT, GL_FALSE, 0, indicatorBackVertices);
    glEnableVertexAttribArray(positionLocIndicator);
    glDrawArrays(GL_TRIANGLES, 0, 9);
}
    
void drawLongNeedle(float rotationAngle)
{
    const float centerX = 0.0f;
    const float centerY = 0.0f;
    const float centerWidth = 0.03f;
     const float innerCenterWidth = 0.02f;
    const float frontHeight = 0.7f;
    const float backHeight = 0.15f;
    const float innerBackHeight = 0.14f;

    float rotationMatrix[4] = {
        cosf(rotationAngle), -sinf(rotationAngle),
        sinf(rotationAngle), cosf(rotationAngle)
    };

    GLfloat indicatorFrontVertices[] = {
        centerX, centerY + frontHeight,
        centerX - centerWidth / 2, centerY + frontHeight - 0.05f,
        centerX + centerWidth / 2, centerY + frontHeight - 0.05f,

        centerX - centerWidth / 2, centerY + frontHeight - 0.05f,
        centerX + centerWidth / 2, centerY + frontHeight - 0.05f,
        centerX + centerWidth / 2, centerY,

        centerX - centerWidth / 2, centerY + frontHeight - 0.05f,
        centerX + centerWidth / 2, centerY,
        centerX - centerWidth / 2, centerY,

        centerX + centerWidth / 2, centerY,
        centerX - centerWidth / 2, centerY,
        centerX - centerWidth / 1.3, centerY - backHeight,

        centerX + centerWidth / 2, centerY,
        centerX - centerWidth / 1.3, centerY - backHeight,
        centerX + centerWidth / 1.3, centerY - backHeight,

        centerX - centerWidth / 1.3, centerY - backHeight,
        centerX + centerWidth / 1.3, centerY - backHeight,
        centerX, centerY - backHeight - 0.02f
    };

    for (int i = 0; i < 18; ++i) {
        float x = indicatorFrontVertices[i * 2];
        float y = indicatorFrontVertices[i * 2 + 1];
        indicatorFrontVertices[i * 2] = x * rotationMatrix[0] + y * rotationMatrix[2];
        indicatorFrontVertices[i * 2 + 1] = x * rotationMatrix[1] + y * rotationMatrix[3];
    }

    glUniform4f(colorLocIndicator, 0.8f, 0.8f, 0.8f, 1.0f);
    glVertexAttribPointer(positionLocIndicator, 2, GL_FLOAT, GL_FALSE, 0, indicatorFrontVertices);
    glEnableVertexAttribArray(positionLocIndicator);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    GLfloat indicatorBackVertices[] = {
        centerX + innerCenterWidth / 2, centerY,
        centerX - innerCenterWidth / 2, centerY,
        centerX - innerCenterWidth / 1.3, centerY - innerBackHeight,

        centerX + innerCenterWidth / 2, centerY,
        centerX - innerCenterWidth / 1.3, centerY - innerBackHeight,
        centerX + innerCenterWidth / 1.3, centerY - innerBackHeight,

        centerX - innerCenterWidth / 1.3, centerY - innerBackHeight,
        centerX + innerCenterWidth / 1.3, centerY - innerBackHeight,
        centerX, centerY - innerBackHeight - 0.02f
    };

    for (int i = 0; i < 9; ++i) {
        float x = indicatorBackVertices[i * 2];
        float y = indicatorBackVertices[i * 2 + 1];
        indicatorBackVertices[i * 2] = x * rotationMatrix[0] + y * rotationMatrix[2];
        indicatorBackVertices[i * 2 + 1] = x * rotationMatrix[1] + y * rotationMatrix[3];
    }

    glUniform4f(colorLocIndicator, 0.0f, 0.0f, 0.0f, 1.0f);
    glVertexAttribPointer(positionLocIndicator, 2, GL_FLOAT, GL_FALSE, 0, indicatorBackVertices);
    glEnableVertexAttribArray(positionLocIndicator);
    glDrawArrays(GL_TRIANGLES, 0, 9);

}

void drawCenterNeedle()
{
    const float centerX = 0.0f;
    const float centerY = 0.0f;
    const float radius = 0.025f;
    const int numSegments = 72;
    const float angleStep = (2.0f * M_PI) / numSegments;

    GLfloat needleVertices[(numSegments + 1) * 2];
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);
        needleVertices[i * 2] = x;
        needleVertices[i * 2 + 1] = y;
    }
    glUniform4f(colorLocIndicator, 0.0f, 0.0f, 0.0f, 1.0f);
    glVertexAttribPointer(positionLocIndicator, 2, GL_FLOAT, GL_FALSE, 0, needleVertices);
    glEnableVertexAttribArray(positionLocIndicator);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    GLfloat littleNeedleVertices[(numSegments + 1) * 2];
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleStep;
        float x = centerX + (radius/2.5) * std::cos(angle);
        float y = centerY + (radius/2.5) * std::sin(angle);
        littleNeedleVertices[i * 2] = x;
        littleNeedleVertices[i * 2 + 1] = y;
    }
    glUniform4f(colorLocIndicator, 1.0f, 1.0f, 1.0f, 1.0f);
    glVertexAttribPointer(positionLocIndicator, 2, GL_FLOAT, GL_FALSE, 0, littleNeedleVertices);
    glEnableVertexAttribArray(positionLocIndicator);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);
}

void drawVisibilityFlag(float x1, float y1, float x2, float y2, float height)
{
    GLfloat trapezoidVertices[] = {
        x1, y1,
        x2, y1,
        x2 - height, y2,
        x1 + height, y2
    };

     if(altitudeValue > 10000.0f)
    {
        glUniform4f(colorLocVisibility, 0.1f, 0.1f, 0.1f, 1.0f);
    }
    else{
        glUniform4f(colorLocVisibility, 1.0f, 1.0f, 1.0f, 1.f);
    }
    
    glVertexAttribPointer(positionLocVisibility, 2, GL_FLOAT, GL_FALSE, 0, trapezoidVertices);
    glEnableVertexAttribArray(positionLocVisibility);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

float calculateRedNeedleAngle(float altitude)
{
    const float minAltitude = 0.0f;
    const float maxAltitude = 100000.0f;
    const float minAngle = 0.0f;
    const float maxAngle = 360.0f;

    float altitudeRange = maxAltitude - minAltitude;
    float altitudeRatio = (altitude - minAltitude) / altitudeRange;
    float angle = minAngle + altitudeRatio * 2 * M_PI;

    return angle;
}

float calculateShortNeedleAngle(float altitude)
{
    const float minAltitude = 0.0f;
    const float maxAltitude = 10000.0f;
    const float minAngle = 0.0f;
    const float maxAngle = 360.0f;

    float altitudeRange = maxAltitude - minAltitude;
    float altitudeRatio = (altitude - minAltitude) / altitudeRange;
    float angle = minAngle + altitudeRatio * 2 * M_PI;

    return angle;
}

float calculateLongNeedleAngle(float altitude)
{
    const float minAltitude = 0.0f;
    const float maxAltitude = 1000.0f;
    const float minAngle = 0.0f;
    const float maxAngle = 360.0f;

    float altitudeRange = maxAltitude - minAltitude;
    float altitudeRatio = (altitude - minAltitude) / altitudeRange;
    float angle = minAngle + altitudeRatio * 2 * M_PI;

    return angle;
}

void drawAltimeter(float currentAltitude)
{

    drawFrame();

    float redAngle = calculateRedNeedleAngle(currentAltitude);
    float shortAngle = calculateShortNeedleAngle(currentAltitude);
    float longAngle = calculateLongNeedleAngle(currentAltitude);

    drawVisibilityFlag(-0.15f, -0.35f, 0.15f, -0.2f, 0.08f);
    drawRedNeedle(redAngle);
    drawShortNeedle(shortAngle);
    drawLongNeedle(longAngle);
    drawCenterNeedle();

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
                if(altitudeValue >=0){
                    altitudeValue += 10;
                }
                break;
            case GLFW_KEY_DOWN:
                if(altitudeValue > 0){
                    altitudeValue -= 10;
                }
                break;
        }
    }
}

int main()
{
    // GLFW'nin başlatılması ve pencere oluşturulması
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(800, 800, "Altimetre", NULL, NULL);
    if (!window)
    {
        std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // Klavye tuşları olayını dinlemek için fonksiyon bağlama
    glfwSetKeyCallback(window, keyCallback);

    // OpenGL programını oluşturma
    createProgram();

    // Texture oluşturma
    GLuint textureID = createTexture("texture.png");

    // Ana döngü
    while (!glfwWindowShouldClose(window))
    {
        // Ekranı temizleme
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        // OpenGL programını kullanma
        glUseProgram(program);

        // Dairesel texture çizme
        drawTexturedCircle(textureID);

        // Altimetre çizme
        drawAltimeter(altitudeValue);

        // Konsola yükseklik değerini yazdırma
        std::cout << "\rAltitude Value: " << altitudeValue << " ft." << std::flush;

        // Pencereyi güncelleme ve olayları işleme
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Pencereyi ve GLFW'yi temizleme
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}