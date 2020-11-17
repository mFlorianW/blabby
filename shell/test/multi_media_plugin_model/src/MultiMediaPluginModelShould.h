#ifndef MULTIMEDIAPLUGINMODELSHOULD_H
#define MULTIMEDIAPLUGINMODELSHOULD_H

#include <QObject>

class MultiMediaPluginModelShould : public QObject
{
    Q_OBJECT
public:
    MultiMediaPluginModelShould();

private Q_SLOTS:
    void give_the_number_of_loaded_multimedia_plugins();
    void give_the_plugin_the_name();
    void give_empty_variant_for_invalid_index();
};

#endif // MULTIMEDIAPLUGINMODELSHOULD_H
