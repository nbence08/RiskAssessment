#include "grantlprivcommand.hpp"
#include "riskeditcontroller.hpp"

void GrantLPrivCommand::execute(){
    descriptor.controller.grantLPrivFromTo(descriptor.from.getModel(), descriptor.to.getModel());
}
