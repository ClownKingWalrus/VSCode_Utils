#include "../hdr/Shader.h"
#include <filesystem>
#include <GL/gl.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    //retreive the vertex/frag source from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    try {
        //open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        //read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        //convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(const std::ifstream::failure& e) {
        std::cout << "Caught ifstream failure" << std::endl  << std::flush;
    } catch (const std::ios_base::failure& e) {
        std::cout << "caught ios base faikure" << e.what() << std::endl  << std::flush;
    } catch (...) {
        std::cout << "caught a failure" << std::endl << std::flush;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cout << "ERROR: Shader source empty. VertexPath=" << vertexPath << " FragmentPath=" << fragmentPath << "\n"; 
        ID = 0;
        return;
    }

    try {
        //start to compile the shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // Vertex Shader
        std::cout << "glCreateShader ptr = " << (void*)glCreateShader << "\n" << std::flush;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);

        // Print compile errors for vertex
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            glDeleteShader(vertex);
            ID = 0;
            return;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);

        // Print compile errors for fragments
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            ID = 0;
            return;
        }

        // Create Shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        
        // Check for link errors
        int successShaderProgram;
        char infoLogShaderProgram[512];
        glGetProgramiv(ID, GL_LINK_STATUS, &successShaderProgram);

        if (!successShaderProgram) {
            glGetProgramInfoLog(ID, 512, NULL, infoLogShaderProgram);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLogShaderProgram << std::endl;
            ID = 0;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } catch (...) {
        std::cout << "opengl was slain in combat" << std::endl << std::flush;
    }
}

void Shader::use() {
    if (ID != 0) {glUseProgram(ID);}
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

Shader::~Shader() {
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}