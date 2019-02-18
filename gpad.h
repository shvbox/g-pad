#ifndef GPAD_H
#define GPAD_H

namespace GPad {
    enum Roles{
        SelectionRole = Qt::UserRole,
        VisibilityRole,
        MoveXYRole,
        MoveZRole,
        MoveDistanceRole,
        MoveArcCenterRole,
        MoveArcRadiusRole,
        MoveArcDirectionRole,
        MoveTypeRole
    };
}

#endif // GPAD_H

