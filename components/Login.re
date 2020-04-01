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
  email: string,
  password: string,
};

let%component make = (~onSubmit, ()) => {
  let%hook ({email, password}, setValue) =
    Hooks.state({email: "", password: ""});

  <View style=[]>
    <Text style=Styles.text text="Username:" />
    <MyInput
      style=[]
      placeholder="email"
      value=email
      onChange={(value, _) => setValue(state => {...state, email: value})}
    />
    <Text style=Styles.text text="Password:" />
    <MyInput
      style=[]
      placeholder="password"
      value=password
      typePassword=true
      onChange={(value, _) => setValue(state => {...state, password: value})}
    />
    <MyButton
      width=100
      fontSize=15.
      title="Submit"
      onClick={() => {
        printf("onClick\n%!");
        onSubmit(~email, ~password);
      }}
    />
  </View>;
};
