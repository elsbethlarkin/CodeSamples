/****************************************************************************/
/*!
\file    Ocean.cpp
\author  Elsbeth Larkin


\brief
This program has functions that make a game similat to battle ship called 
Warboats where the computer picks spots to kill ships :

- Ocean
- TakeShot
- BoatPlacement
- GetShotStats
- GetGrid
- GetPosition

*/
/****************************************************************************/

#include "WarBoats.h"
#include "Ocean.h"



  namespace WarBoats
  {
      /*****************************************************************************/
    /*!
      \brief
        it's the constructor for the class ocean
      
      \param num_boats
        number of boats that will be placed in the ocean

      \param x_quadrants
        the width of the ocean being created

      \param y_quadrants
        the height of the ocean being created
        
    */
/*****************************************************************************/
    Ocean::Ocean(int num_boats, int x_quadrants = 10, int y_quadrants = 10)
    {
      x_quadrants_ = x_quadrants; //width of the grid
      y_quadrants_ = y_quadrants;// height og the grid
      grid_ = new int[y_quadrants_*x_quadrants_]; // the ocean grid

      for(int i = 0; i < y_quadrants * x_quadrants; i++)
        grid_[i] = 0;// sets everything to 0 in the grid
      num_boats_ = num_boats; // number of boats in the ocean
      boats_ = new Boat[num_boats_]; // list of boats in the ocean

	  //sets all the stats to 0 for the ocean
      stats_.hits = 0; 
      stats_.misses = 0;
      stats_.duplicates = 0;
      stats_.sunk = 0;

  }
/*****************************************************************************/
/*!
    \brief
      Destroys the ocean.
*/
/*****************************************************************************/
  Ocean::~Ocean()
  {
	  // removes the grid and boats so that there isn't a memory leak
    delete[] grid_;
    delete[] boats_;
  }
  /*****************************************************************************/
    /*!
      \brief
        tells what happens when the shot was made at an specific spot.
      
      \param coordinate
        the point that is being shot at.
        
      \return
        the result of the shot that was made in the ocean.
    */
/*****************************************************************************/
        ShotResult Ocean::TakeShot(const Point &coordinate)
        {
         // BEGIN FUNCTION

     //IF the shot is out of the ocean (grid) THEN
     //Return Illegal shot result
     //END
          if(x_quadrants_ < coordinate.x || 0 > coordinate.x ||
             y_quadrants_ < coordinate.y || 0 > coordinate.y)
          {
            return srILLEGAL;
          }

      //  Read value at position of shot from grid
          int array_value = coordinate.y * x_quadrants_ + coordinate.x;
          int value = grid_[array_value];

      //  IF value is OK THEN
      //   Increment misses
      //   Set position to Blownup
      //   Return Miss shot result
      // END
          if(value == 0)
          {
            stats_.misses++;
            grid_[array_value] = dtBLOWNUP;
            return srMISS;
          }

     //IF value is a Duplicate THEN
     //Increment duplicates
     //Return Duplicate shot result
     //END
          if(value > 100 || value < 0)
          {
            stats_.duplicates++;
            return srDUPLICATE;
          }
     //Increment hits for this boat
     //Update position to boat hit
          else
          {
        
            boats_[value - 1].hits++;
            grid_[array_value] += 100;
            stats_.hits++;
            //IF number of hits for this boat is >= BOAT_LENGTH 
            //THEN Increment sunk
            // return Sunk shot result
            //END
            if(boats_[value - 1].hits >= BOAT_LENGTH)
            {
              stats_.sunk++;
              return srSUNK;
            }
      //Return Hit shot result
            return srHIT;
          }
    //END FUNCTION
        }

        /*****************************************************************************/
    /*!
    \brief
    Places boats in the ocean 

    \param boat
    the boat being placed in the ocean

    \return
    returns weither or not the baot was actually placed in the ocean.
    */
/*****************************************************************************/
     BoatPlacement Ocean::PlaceBoat(const Boat& boat)
        {
          //checks if the position is outside of the grid for vertical boats.
          bool check_position_v = boat.position.y > (y_quadrants_ - BOAT_LENGTH) 
            || boat.position.y < 0
            || boat.position.x < 0 
            || boat.position.x > x_quadrants_;
          //checks if the position is outside of the grid for horizontal boats.
          bool check_position_h = boat.position.x > (x_quadrants_ - BOAT_LENGTH)
            || boat.position.x < 0 
            || boat.position.y < 0 
            || boat.position.y > y_quadrants_;

      //checks the orientation
          if(boat.orientation == oHORIZONTAL)
          {
        //checks to see if the position is too big
        //returns a rejection if too big
            if(check_position_h)
            {
              return bpREJECTED;
            }
            else
            {
        //finds the right array value in Ocean
              int array_value = boat.position.y *
               x_quadrants_ +
               boat.position.x;
        // makes the boat in the ocean
              for (int i = 0; i < BOAT_LENGTH; ++i)
              {
          //checks to see if it hits another boat.
                if(grid_[array_value + i])
                {
          // clears up all previous id's made
                  for(int j = 0; j < i; j++)
                    grid_[array_value + j] = 0;
                  // rejects the placement
                  return bpREJECTED;
                }
                else
                {
          //puts the number id down on the ocean
                  grid_[array_value + i] = boat.ID;
                }
              }
        //puts the boat in the ocean object
        boats_[boat.ID - 1] = boat;
        boats_[boat.ID - 1].hits = 0;
        
              return bpACCEPTED;
            }
          }
          else // vertical
          {
            if(check_position_v)
            {
              // rejects the boat if outside of the 
              return bpREJECTED;
            }
            else
            {
        // makes the boat in the ocean
              for (int i = 0; i < BOAT_LENGTH; ++i)
              {
                //checks to see if it hits another boat.
                if(grid_[(boat.position.y + i) *
                    x_quadrants_ +
                    boat.position.x])
                {
        // clears up all previous id's made
                for(int j = 0; j < i; j++)
                  {
                    //makes the places on the grid 0 if another boat 
                    //intercescts
                    grid_[(boat.position.y + j) *
                    x_quadrants_ +
                    boat.position.x] = 0;
                  }
                  return bpREJECTED;
                }
                else
                {
        //puts the number id down on the ocean
                  grid_[(boat.position.y + i) *
                  x_quadrants_ +
                  boat.position.x] = boat.ID;
                }
              }
        //puts the boat in the ocean object
        boats_[boat.ID - 1] = boat;
        //makes the boat have 0 hits.
        boats_[boat.ID - 1].hits = 0;
              return bpACCEPTED;
            }
          }
        }
/*****************************************************************************/
/*!
    \brief
      gets the stats of the ocean

    \return
      returns the stats that the ocean has.
*/
/*****************************************************************************/
        ShotStats Ocean::GetShotStats() const
        {
      //returns stats
          return stats_;
        }
 
/*****************************************************************************/
/*!
    \brief
      gets the grid of the ocean

    \return
      returns a pointer to the first term in the grid
*/
/*****************************************************************************/
  const int *Ocean::GetGrid() const
  {
    return grid_;
  }
/*****************************************************************************/
/*!
    \brief
      gets the x and y quardinats for the grid or how big the grid is 

    \return
      returns a point for the max x and y values
*/
/*****************************************************************************/
  Point Ocean::GetDimensions() const
  {
    Point pt = {x_quadrants_, y_quadrants_};
    return pt;
  }

} // namespace WarBoats



