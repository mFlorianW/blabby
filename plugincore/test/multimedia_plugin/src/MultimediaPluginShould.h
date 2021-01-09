#ifndef MULTIMEDIAPLUGINSHOULD_H
#define MULTIMEDIAPLUGINSHOULD_H

#include <QObject>

namespace PluginCore
{

class MultimediaPluginShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MultimediaPluginShould)
public:
    explicit MultimediaPluginShould(QObject *parent = nullptr);

private Q_SLOTS:
    void be_able_to_set_a_active_view();
};

} // namespace PluginCore

#endif // MULTIMEDIAPLUGINSHOULD_H
