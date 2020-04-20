open Revery;
open Revery.UI;
open Revery.UI.Components;

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
    backgroundColor(Colors.white),
  ];

module Example = {
  type inputFields = {
    first: string,
    second: string,
  };

  let%component make = () => {
    let%hook ({first, _}, setValue) = Hooks.state({first: "", second: ""});

    <View style=containerStyle>
      <View
        style=Style.[
          flexDirection(`Row),
          alignItems(`Center),
          justifyContent(`Center),
        ]>
        <Input
          placeholder="Insert text here"
          onChange={(value, _) =>
            setValue(state => {...state, first: value})
          }
          value=first
        />
        <MyButton
          width=100
          fontSize=15.
          title="Reset"
          onClick={() => setValue(state => {...state, first: ""})}
        />
        <MyButton
          width=100
          fontSize=15.
          title="Set value"
          onClick={() => setValue(state => {...state, first: "New value"})}
        />
      </View>
      <Padding padding=20>
        <BoxShadow
          boxShadow={Style.BoxShadow.make(
            ~xOffset=-5.,
            ~yOffset=2.,
            ~color=Colors.black,
            ~blurRadius=20.,
            ~spreadRadius=0.,
            (),
          )}>
          <Input
            placeholder="custom input"
            placeholderColor=Colors.plum
            cursorColor=Colors.white
            autofocus=true
            onFocus={() => Console.log("Input example focused")}
            onBlur={() => Console.log("Input example blurred")}
            onChange={(value, _) =>
              setValue(state => {...state, second: value})
            }
            onKeyDown={event => Console.log(event)}
            style=Style.[
              backgroundColor(Colors.paleVioletRed),
              color(Colors.white),
              height(50),
            ]
          />
        </BoxShadow>
      </Padding>
    </View>;
  };
};
