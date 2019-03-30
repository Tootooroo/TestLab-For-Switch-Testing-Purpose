/**************************************************************
 * Concept                                                    *
 **************************************************************
 * This file contain concepts that will be frequently used in *
 * this project.                                              *
 **************************************************************
 * Predicate(P) ::= {
 *   Procedure(P) &&
 *   Codomain(P) == _Bool &&
 *   Arity(P) > 0
 * }
 *
 * Operation(O) ::= {
 *   Procedure(P) &&
 *   Domain(P) == Codomain(P)
 * }
 *   
 * Action(A, R, S, F) ::= {
 *   Procedure(P) &&
 *   Codomain(P) == R
 *   Success(A)  => Return(A, S)
 *   !Success(A) => Return(A, F)
 * }
 *
 */

