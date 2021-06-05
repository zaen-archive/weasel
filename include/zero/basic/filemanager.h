namespace zero
{
    class FileManager
    {
    protected:
        char *_begin;
        char *_end;
        bool _ok = true;
        size_t _size;

    private:
        // I copy paste the implementation from
        // https://github.com/d0k/mappedfile
        char *mapFile(const char *path, size_t *length);

    public:
        FileManager(const char *filePath);

        bool isValid() const { return _ok; }
        size_t getSize() const { return _size; }
        char *getBegin() const { return _begin; }
        char *getEnd() const { return _end; }
    };

} // namespace zero
