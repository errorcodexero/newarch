#include "XeroPathManager.h"
#include "MessageLogger.h"
#include "MessageDestStream.h"

using namespace xero::misc ;

int main(int ac, char **av)
{
    const char *name_p = "HabCenterToShipFrontLeft" ;
    MessageLogger logger ;
    std::shared_ptr<MessageLoggerDest> dest = std::make_shared<MessageDestStream>(std::cout) ;

    logger.addDestination(dest) ;
    XeroPathManager mgr(logger, "paths") ;

    if (!mgr.loadPath(name_p)) {
        std::cout << "Failed: did not load path" << std::endl ;
        return 1 ;
    }

    auto path = mgr.getPath(name_p) ;
    if (path == nullptr) {
        std::cout << "Failed: did not find loaded path" << std::endl ;
        return 1 ;
    }

    std::cout << "Path " << name_p << " has " << path->size() << " segments" << std::endl ;

    for(size_t i = 0 ; i < path->size() ; i++) {
        const XeroPathSegment &left = path->getLeft(i) ;
        const XeroPathSegment &right = path->getRight(i) ;

        std::cout << "left" ;
        std::cout << " " << left.getX() ;
        std::cout << " " << left.getY() ;
        std::cout << " " << left.getPOS() ;
        std::cout << " " << left.getVelocity() ;
        std::cout << " " << left.getAcceleration() ;
        std::cout << " " << left.getJerk() ;
        std::cout << " " << left.getHeading() ;
        std::cout << " right" ;
        std::cout << " " << right.getX() ;
        std::cout << " " << right.getY() ;
        std::cout << " " << right.getPOS() ;
        std::cout << " " << right.getVelocity() ;
        std::cout << " " << right.getAcceleration() ;
        std::cout << " " << right.getJerk() ;
        std::cout << " " << right.getHeading() ;   
        std::cout << std::endl ;     

    }
    return 0 ;
}