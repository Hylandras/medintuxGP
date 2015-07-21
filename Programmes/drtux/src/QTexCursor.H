class Q_EXPORT QTextCursor
{
public:
    QTextCursor( QTextDocument *d = 0 );
    QTextCursor( const QTextCursor &c );
    QTextCursor &operator=( const QTextCursor &c );
    virtual ~QTextCursor() {}

    bool operator==( const QTextCursor &c ) const;
    bool operator!=( const QTextCursor &c ) const { return !(*this == c); }

#if defined(Q_STRICT_INLINING_RULES)
    // This is for the IRIX MIPSpro o32 ABI - it fails, claiming the
    // implementation to be a redefinition.
    inline QTextParagraph *paragraph() const;
#else
    QTextParagraph *paragraph() const;
#endif

    void setParagraph( QTextParagraph*p ) { gotoPosition(p, 0 ); }
    QTextDocument *document() const;
    int index() const;
    void setIndex( int index ) { gotoPosition(paragraph(), index ); }

    void gotoPosition( QTextParagraph* p, int index = 0);
    void gotoLeft();
    void gotoRight();
    void gotoNextLetter();
    void gotoPreviousLetter();
    void gotoUp();
    void gotoDown();
    void gotoLineEnd();
    void gotoLineStart();
    void gotoHome();
    void gotoEnd();
    void gotoPageUp( int visibleHeight );
    void gotoPageDown( int visibleHeight );
    void gotoNextWord( bool onlySpace = FALSE );
    void gotoPreviousWord( bool onlySpace = FALSE );
    void gotoWordLeft();
    void gotoWordRight();

    void insert( const QString &s, bool checkNewLine, QMemArray<QTextStringChar> *formatting = 0 );
    void splitAndInsertEmptyParagraph( bool ind = TRUE, bool updateIds = TRUE );
    bool remove();
    void indent();

    bool atParagStart();
    bool atParagEnd();

    int x() const; // x in current paragraph
    int y() const; // y in current paragraph

    int globalX() const;
    int globalY() const;

    QTextParagraph *topParagraph() const { return paras.isEmpty() ? para : paras.first(); }
    int offsetX() const { return ox; } // inner document  offset
    int offsetY() const { return oy; } // inner document offset
    int totalOffsetX() const; // total document offset
    int totalOffsetY() const; // total document offset

    bool place( const QPoint &pos, QTextParagraph *s ) { return place( pos, s, FALSE ); }
    bool place( const QPoint &pos, QTextParagraph *s, bool link );
    void restoreState();


    int nestedDepth() const { return (int)indices.count(); } //### size_t/int cast
    void oneUp() { if ( !indices.isEmpty() ) pop(); }
    void setValid( bool b ) { valid = b; }
    bool isValid() const { return valid; }

private:
    enum Operation { EnterBegin, EnterEnd, Next, Prev, Up, Down };

    void push();
    void pop();
    void processNesting( Operation op );
    void invalidateNested();
    void gotoIntoNested( const QPoint &globalPos );

    QTextParagraph *para;
    int idx, tmpIndex;
    int ox, oy;
    QValueStack<int> indices;
    QValueStack<QTextParagraph*> paras;
    QValueStack<int> xOffsets;
    QValueStack<int> yOffsets;
    uint valid : 1;

};