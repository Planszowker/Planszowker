# Board Description JSON

1. [Board Description JSON](#board-description-json-1)
   1. [Fields](#fields)
      1. [Board](#board)
      2. [PlayersView](#playersview)
         1. [Fields](#fields-1)
      3. [Destination Points](#destination-points)
      4. [Entities](#entities)
      5. [Tiles](#tiles)


## Board Description JSON
Describes how game's board is defined. This JSON contains informations about board itself, required entities and fixed points, and other game specific objects.

### Fields

#### Board
Describes board's background. If not specified, `default` is implicitly used.

#### PlayersView
Describes players' view. If game need to be able to view other players' views, one can define where this view is located and how big it is.
##### Fields
###### Side
Describe on which side players' view will be attached. Possible values: `Bottom`, `Top`, `Left` or `Right`.
###### Size
Size of players' view in format `<X>x<Y>` (e.g. `1.0x0.4`), where:
- `<X>` is size in X-axis,
- `<Y>` is size in Y-axis.

#### Destination Points
To be considered as fixed points.
TODO

#### Entities
To be considered as sprites with given texture.
TODO

#### Tiles
To be considered as not-so-fixed points. Can be attached to other objects.
TODO