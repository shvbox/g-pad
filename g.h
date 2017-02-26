#ifndef G
#define G

namespace G {
    enum Roles{
        SelectionRole = Qt::UserRole,
        VisibilityRole,
        MoveXYRole,
        MoveZRole,
        MoveTypeRole
    };
}

namespace Units {
    enum SpeedUnits {
        mmPerMin,
        mmPerS
    };
}

#endif // G

