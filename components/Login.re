open Stdio;
open Core;
open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let text = [
    color(Colors.white),
    fontFamily(Constants.fontFamily),
    fontSize(20.),
  ];
};

type inputFields = {
  username: string,
  password: string,
};

let%component make = (~onSubmit, ()) => {
  let%hook ({username, password}, setValue) =
    Hooks.state({username: "", password: ""});

  <View style=[]>
    <Text style=Styles.text text="Username:" />
    <Input2
      style=[]
      placeholder="username"
      value=username
      onChange={(value, _) => setValue(state => {...state, username: value})}
    />
    <Text style=Styles.text text="Password:" />
    <Input2
      style=[]
      placeholder="password"
      value=password
      onChange={(value, _) => setValue(state => {...state, password: value})}
    />
    <MyButton
      width=100
      fontSize=15.
      title="Submit"
      onClick={() => {
        printf("onClick\n%!");
        onSubmit(~username, ~password);
      }}
    />
  </View>;
};
