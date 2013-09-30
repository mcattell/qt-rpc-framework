#include "controller.h"
#include "metaservice.h"

MetaService* Controller::metaService() const {
    return m_metaService;
}
Controller::ControllerState Controller::state() const {

    return m_state;
}
void Controller::setState(ControllerState state) {

    m_state = state;
    emit stateChanged(m_state, metaService()->uuid());
}
