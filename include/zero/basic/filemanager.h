#pragma once

namespace zero
{
    class FileManager
    {
    protected:
        char *_startBuffer;
        unsigned long long _size;

    private:
        // I copy paste the implementation from
        // https://github.com/d0k/mappedfile
        char *mapFile(const char *path, size_t *length);

    public:
        FileManager(const char *filePath);

        bool isValid() const { return _startBuffer != nullptr; }
        unsigned long long getSize() const { return _size; }
        char *getStartBuffer() const { return _startBuffer; }
    };

} // namespace zero
