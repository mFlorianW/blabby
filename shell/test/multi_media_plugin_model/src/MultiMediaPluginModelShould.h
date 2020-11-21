#ifndef MULTIMEDIAPLUGINMODELSHOULD_H
#define MULTIMEDIAPLUGINMODELSHOULD_H

#include <QObject>

class MultiMediaPluginModelShould : public QObject
{
    Q_OBJECT
public:
    MultiMediaPluginModelShould();

private Q_SLOTS:
    void give_the_correct_role_names();
    void give_the_number_of_loaded_multimedia_plugins();
    void give_the_plugin_the_name();
    void give_empty_variant_for_invalid_index();
    void give_qml_url_for_valid_index();
    void give_status_if_a_plugin_is_active();
    void give_a_icon_url_for_a_plugin();
};

#endif // MULTIMEDIAPLUGINMODELSHOULD_H
