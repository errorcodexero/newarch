#include "DebounceBoolean.h"
#include <catch.h>

using namespace xero::misc ;

TEST_CASE("DebounceBoolean", "BasicTest")
{
    SECTION("Less Than One Loop - Active High")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(false, 0.001) ;
        REQUIRE(debounce.get() == false) ;
        debounce.update(true, 0.0) ;
        REQUIRE(debounce.get() == false) ;        
        debounce.update(true, 0.02) ;
        REQUIRE(debounce.get() == true) ;        
    }

    SECTION("Less Than One Loop - Active Low")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(true, 0.001) ;
        REQUIRE(debounce.get() == true) ;
        debounce.update(false, 0.0) ;
        REQUIRE(debounce.get() == true) ;        
        debounce.update(false, 0.02) ;
        REQUIRE(debounce.get() == false) ;        
    }    

    SECTION("More Than One Loop - Active High")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(false, 0.03) ;
        REQUIRE(debounce.get() == false) ;
        debounce.update(true, 0.0) ;
        REQUIRE(debounce.get() == false) ;        
        debounce.update(true, 0.02) ;
        REQUIRE(debounce.get() == false) ;  
        debounce.update(true, 0.04) ;
        REQUIRE(debounce.get() == true) ;
    }

    SECTION("Less Than One Loop - Active Low")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(true, 0.03) ;
        REQUIRE(debounce.get() == true) ;
        debounce.update(false, 0.0) ;
        REQUIRE(debounce.get() == true) ;        
        debounce.update(false, 0.02) ;
        REQUIRE(debounce.get() == true) ;    
        debounce.update(false, 0.04) ;
        REQUIRE(debounce.get() == false) ;               
    }       

    SECTION("Filter - Active High")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(false, 0.03) ;
        REQUIRE(debounce.get() == false) ;
        debounce.update(true, 0.0) ;
        REQUIRE(debounce.get() == false) ;        
        debounce.update(true, 0.02) ;
        REQUIRE(debounce.get() == false) ;  
        debounce.update(false, 0.04) ;
        REQUIRE(debounce.get() == false) ;
    }

    SECTION("Filter - Active Low")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(true, 0.03) ;
        REQUIRE(debounce.get() == true) ;
        debounce.update(false, 0.0) ;
        REQUIRE(debounce.get() == true) ;        
        debounce.update(false, 0.02) ;
        REQUIRE(debounce.get() == true) ;    
        debounce.update(true, 0.04) ;
        REQUIRE(debounce.get() == true) ;               
    }     

    SECTION("Async - Active High")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(false, 0.03, 0.05) ;
        REQUIRE(debounce.get() == false) ;
        debounce.update(true, 0.0) ;
        REQUIRE(debounce.get() == false) ;        
        debounce.update(true, 0.02) ;
        REQUIRE(debounce.get() == false) ;    
        debounce.update(true, 0.04) ;
        REQUIRE(debounce.get() == false) ;       
        debounce.update(true, 0.06) ;
        REQUIRE(debounce.get() == true) ;    
        debounce.update(false, 0.08) ;   
        REQUIRE(debounce.get() == true) ;  
        debounce.update(false, 0.10) ;   
        REQUIRE(debounce.get() == true) ;                                  
        debounce.update(false, 0.12) ;   
        REQUIRE(debounce.get() == false) ;         
    }

    SECTION("Async - Active Low")
    {
        //
        // Debounce delay less than a single robot loop
        //
        DebounceBoolean debounce(true, 0.05, 0.03) ;
        REQUIRE(debounce.get() == true) ;
        debounce.update(false, 0.0) ;
        REQUIRE(debounce.get() == true) ;        
        debounce.update(false, 0.02) ;
        REQUIRE(debounce.get() == true) ;    
        debounce.update(false, 0.04) ;
        REQUIRE(debounce.get() == true) ;       
        debounce.update(false, 0.06) ;
        REQUIRE(debounce.get() == false) ;    
        debounce.update(true, 0.08) ;   
        REQUIRE(debounce.get() == false) ;  
        debounce.update(true, 0.10) ;   
        REQUIRE(debounce.get() == false) ;                                  
        debounce.update(true, 0.12) ;   
        REQUIRE(debounce.get() == true) ;         
    }    
}
