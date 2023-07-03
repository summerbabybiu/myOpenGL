class IndexBuffer
{
private:
    /* data */
    unsigned int m_RendererID;
    unsigned int m_Count; // index count of the buffer 多少个索引
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;

    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }

};