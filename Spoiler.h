#ifndef SPOILER_H
#define SPOILER_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

class Section : public QWidget
{
    Q_OBJECT

private:
    QGridLayout* mainLayout;
   // QToolButton* toggleButton;
    QFrame* headerLine;
    QParallelAnimationGroup* toggleAnimation;
    QScrollArea* contentArea;
    int animationDuration;
    int collapsedHeight;
    bool isExpanded = false;

public slots:
    void toggle(bool collapsed);

public:
    static const int DEFAULT_DURATION = 0;

    // initialize section
    explicit Section(const QString& title = "", const int animationDuration = DEFAULT_DURATION, QWidget* parent = 0);

    // set layout of content
    void setContentLayout(QLayout& contentLayout);

    // set title
    void setTitle(QString title);

    // update animations and their heights
    void updateHeights();

    QToolButton* toggleButton;
};
#endif // SPOILER_H

