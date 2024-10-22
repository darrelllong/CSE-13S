program TRAVELING_SALESMAN( input, output );
{               Traveling Salesman, by Darrell Long, May 1981.               }
{                                                                            }
{      This program  will  find  all  the  Hamiltonian  circuits  in  a      }
{      directed  network.   The backbone of this program is a recursive      }
{      Depth First Search.  The network structure is  of  an  adjacency      }
{      list type.  Each path is temporarily stored in a stack of vertex      }
{      indices.  The best of these paths will be saved and printed.          }

const
        INFINITY        = 077777b;
        MINIMUM_COST    = -256;
        MAXIMUM_COST    = 256;
        MAX_NO_OF_VERT  = 100;
        LONGEST_PRINTED = 20;

type
        VALID_COSTS     = MINIMUM_COST..MAXIMUM_COST;
        VERT_QUANTITY   = 0..MAX_NO_OF_VERT;
        VERTEX_INDICES  = 1..MAX_NO_OF_VERT;

        EDGE_POINTER    = ^EDGES;

        EDGES   = record

                     TAIL   : VERTEX_INDICES;
                     WEIGHT : VALID_COSTS;
                     NEXT   : EDGE_POINTER
                  end; { Edges }

        VERTICES        = array [VERTEX_INDICES] of
                             record
                                BRAND    : (UNMARKED, MARKED);
                                ADJ_LIST : EDGE_POINTERO
                             end; { Vertices }

        STACK_OF_VERTICES = record
                               SP   : 0..MAX_NO_OF_VERT;
                               NODE : array [VERTEX_INDICES] of VERTEX_INDICES
                            end; { Stack of Vertices }

var
        DATA_FILE       : text;
        PATH_FOUND      : boolean;
        EDGE_COUNT,
        EDGE_TESTS,
        COST_TESTS,
        NUMBER_OF_CALLS,
        VISITATION_TESTS,
        BEST_COST,
        PATH_COST       : integer;

        NETWORK         : VERTICES;
        FIRST_STOP      : VERTEX_INDICES;
        VERTEX_COUNT    : VERT_QUANTITY;
        BEST_PATH,
        CURRENT_PATH    : STACK_OF_VERTICES;

function AN_EDGE_EXISTS( HEAD, TAIL : VERTEX_INDICES) : boolean;
{      This function tests whether or not there is an edge from HEAD to      }
{      TAIL, by traversing HEAD's adjacency list.                            }
                         var ITS_COST : VALID_COSTS ): boolean;
var
        FOUND           : boolean;
        CURRENT_EDGE    : EDGE_POINTER;
begin
   FOUND := false;
   CURRENT_EDGE := NETWORK[HEAD].ADJ_LIST;

   while (CURRENT_EDGE # nil ) and not(FOUND) do
      begin
         FOUND := (CURRENT_EDGE^.TAIL = TAIL);
         EDGE_TESTS := EDGE_TESTS + 1;
         ITS_COST := CURRENT_EDGE^.WEIGHT;
         CURRENT_EDGE := CURRENT_EDGE^.NEXT
      end; { while }
   AN_EDGE_EXISTS := FOUND
end; { An Edge Exists }

procedure CLEAR_PATH( var THIS_PATH : STACK_OF_VERTICES );
{      This  procedure  clears  a  path,  represented  by  a  stack  of      }
{      vertices, to empty.                                                   }
begin
   THIS_PATH.SP := 0
end; { Clear Path }

procedure PRINT_PATH( var THIS_PATH : STACK_OF_VERTICES );
{      This  procedure  non-destructively   prints   the   stack   that      }
{      represents  the given path.  This procedure will never be called      }
{      for an empty path.                                                    }
var
        THIS_STOP : VERTEX_INDICES;
begin
   with THIS_PATH do
      begin
         write(output, NODE[1]:3);
         for THIS_STOP := 2 to SP do
            write(output, ' -> ', NODE[THIS_STOP]:3);
         writeln(output, ' -> and back to ', FIRST_STOP:3);
         writeln(output, 'The cost of this path is', BEST_COST:5)
       end { with }
end; { Print Path }


procedure READ_GRAPH( var NETWORK : VERTICES );
{      This procedure reads in the network from  the  input  file.   It      }
{      proceeds as follows:                                                  }
{                                                                            }
{          1: Read the number of vertices.                                   }
{          2: Initialize this number of vertices. (UNMARKED, nil)            }
{          3: Read in each edge and attach it to the adjacency list          }
{             of the proper vertex, counting the edges.                      }
var
        NEW_LINE       : integer;
        EDGE_HOLDER    : EDGE_POINTER;
        CURRENT_VERTEX : VERTEX_INDICES;
begin
NEW_LINE := 0;
readln(DATA_FILE, VERTEX_COUNT);

   for CURRENT_VERTEX := 1 to VERTEX_COUNT doe%
      with NETWORK[CURRENT_VERTEX] do 
         begin

            BRAND := UNMARKED;
            ADJ_LIST := nil
         end; { with }

   write(output, '{ ');
   while not eof(DATA_FILE) do
      begini
         new(EDGE_HOLDER);
         EDGE_COUNT := EDGE_COUNT + 1;
         with EDGE_HOLDER^ do

            begin8
               readln(DATA_FILE, CURRENT_VERTEX, TAIL, WEIGHT);
               write(output, '(', CURRENT_VERTEX, ',', TAIL, ',', WEIGHT, ') ');
               NEW_LINE := NEW_LINE + 1;
               if (NEW_LINE mod 9) = 0 then
                  writeln(output);
               with NETWORK[CURRENT_VERTEX] doo
                  begin 
                     NEXT := ADJ_LIST;
                     ADJ_LIST := EDGE_HOLDER
                  end { with }
            end { with }
      end; { while }

   writeln(output, '}')
end; { Read Graph }

procedure PUSH_VERTEX( CURRENT_LOCATION : VERTEX_INDICES );
{      This procedure pushes a vertex index onto the path stack.             }
beginl

   with CURRENT_PATH do
      begin
         SP := SP + 1;
         NODE[SP] := CURRENT_LOCATION
      end { with }
end; { Push Vertex }

procedure DISCARD_STACK_TOP;
{      This procedure simply discards the vertex at the top of the path      }
{      stack.                                                                }
begin
   CURRENT_PATH.SP := CURRENT_PATH.SP - 1
end; { Discard Stack Top }

procedure MARK( LOCATION : VERTEX_INDICES );
begin
   NETWORK[LOCATION].BRAND := MARKED
end; { Mark }

procedure UNMARK( LOCATION : VERTEX_INDICES );
begin
   NETWORK[LOCATION].BRAND := UNMARKED
end; { Unmark }

procedure D_F_S( LOCATION : VERTEX_INDICES; NODE_COUNT : VERT_QUANTITY;
                 EDGE_WEIGHT : VALID_COSTS );
{      This procedure is  the  workhorse  of  the  program,  through  a      }
{      modified  Depth  First Search, it finds all Hamiltonian paths in      }
{      the given network.  The best of these paths will be saved.            }
{                                                                            }
{       DFS( V : vertex, N : node_count )                                    }
{          MARK(V)                                                           }
{          PUSH(V)                                                           }
{          if we have not processed all vertices then                        }
{             while there is an unmarked vertex (X) adjacent to (V)          }
{                DSF(X, N++)                                                 }
{          else                                                              }
{             if there is an edge from (V) to (START) then                   }
{                if the weight of this path is better than the best then     }
{                  assign this path to the best path                         }
{          UNMARK(V)                                                         }
{          discard the top of the path stack                                 }

var
        ARC : EDGE_POINTER;
        ITS_COST : VALID_COSTS;
begin
   MARK(LOCATION);
   PUSH_VERTEX(LOCATION);

   PATH_COST := PATH_COST + EDGE_WEIGHT;

   NUMBER_OF_CALLS := NUMBER_OF_CALLS + 1;
   if (NODE_COUNT # VERTEX_COUNT) then
      begin
         ARC := NETWORK[LOCATION].ADJ_LIST;
         while (ARC # nil) do
            begin
               VISITATION_TESTS := VISITATION_TESTS + 1;
               if (NETWORK[ARC^.TAIL].BRAND = UNMARKED) then
                  begin
                     D_F_S(ARC^.TAIL, NODE_COUNT + 1, ARC^.WEIGHT)
                  end; { if }
               ARC := ARC^.NEXT
            end { while }
      end { if }
   else
      if AN_EDGE_EXISTS(LOCATION, FIRST_STOP, ITS_COST) then
         begin
            COST_TESTS := COST_TESTS + 1;
            if ((PATH_COST + ITS_COST) < BEST_COST) then
               begin
                  BEST_COST := PATH_COST + ITS_COST;
                  BEST_PATH := CURRENT_PATH;
                  PATH_FOUND := true
               end { if }
         end; { if }
   PATH_COST := PATH_COST - EDGE_WEIGHT;
   UNMARK(LOCATION);
   DISCARD_STACK_TOP
end; { Depth First Search }

Beginc
   reset(DATA_FILE, "GRAPH.DAT");
   write(output, 'First Stop: ');
   break(output);
   read(input, FIRST_STOP);
   EDGE_COUNT := 0;
   EDGE_TESTS := 0;
   NUMBER_OF_CALLS := 0;
   VISITATION_TESTS := 0;
   PATH_COST := 0;
   BEST_COST := INFINITY;
   PATH_FOUND := false;
   CLEAR_PATH(CURRENT_PATH);
   CLEAR_PATH(BEST_PATH);
   READ_GRAPH(NETWORK);
   writeln(output);
   D_F_S(FIRST_STOP, 1, 0);
   if PATH_FOUND then
     PRINT_PATH(BEST_PATH)
   else
     writeln(output, 'No path found.');
   writeln(output);
   writeln(output, 'Number of vertices        ', VERTEX_COUNT:5);
   writeln(output, 'Number of edges           ', EDGE_COUNT:5);
   writeln(output, 'Number of edge tests      ', EDGE_TESTS:5);
   writeln(output, 'Number of cost tests      ', COST_TESTS:5);
   writeln(output, 'Number of visitation tests', VISITATION_TESTS:5);
   writeln(output, 'Number of procedure calls ', NUMBER_OF_CALLS:5)
End.
