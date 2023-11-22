#define ROTATE_SPEED     30
/*
 *  Przykładowy zestaw poleceń
 */
  BEGIN_PARALLEL_ACTIONS
  Set   Podstawa 2 0 30    // Polozenie obiektu A 
  Set   Podstawa.Ramie1 10 10 0   // Polozenie obiektu B
  END_PARALLEL_ACTIONS

  BEGIN_PARALLEL_ACTIONS
  Pause 1000 /* Zawieszenie na 1 sek. */
  END_PARALLEL_ACTIONS

  BEGIN_PARALLEL_ACTIONS
  Move  Podstawa.Ramie1.Ramie2  10 10
  Rotate Podstawa.Ramie1.Ramie2 ROTATE_SPEED 90 
  END_PARALLEL_ACTIONS
