#ifndef DELETABLEDIAGRAMELEMENT_H
#define DELETABLEDIAGRAMELEMENT_H

//  view deletion and (model+view) deletion is separated on view objects
//      this class provides an interface for the altogether deletion (model+view)
//      therefore, deleteElement should always call deletion of itself, and notify
//      the controller, and the model of the deletion, and delete the model too
class DeletableDiagramElement{
public:
    virtual void deleteElement() = 0;
};

#endif // DELETABLEDIAGRAMELEMENT_H
