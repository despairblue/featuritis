open Revery;
open Revery.UI;
open MyComponents.Components;

let printHealthAndExit = () => {
  Stdio.(printf("Hi, this works!\n%!"));
  exit(0);
};

Arg.parse([("--checkhealth", Unit(printHealthAndExit), "")], _ => (), "");

let main = () => {
  module Styles = {
    open Style;

    let container = [
      flexDirection(`Column),
      justifyContent(`Center),
      alignItems(`Center),
    ];

    let inner = [flexDirection(`Row), alignItems(`FlexEnd)];
  };

  let config = Core.Config.readConfig();

  <View
    style=Style.[
      flexDirection(`Column),
      flexGrow(1),
      alignItems(`Stretch),
      alignSelf(`Stretch),
      padding(20),
    ]>
    <View style=Styles.container>
      <View style=Styles.inner>
        <AnimatedText delay={Time.ms(0)} text="Welcome" />
        <AnimatedText delay={Time.ms(500)} text="to" />
        <AnimatedText delay={Time.ms(1000)} text="Featuritis" />
      </View>
    </View>
    // <MyComponents.GraphqlHookTest config />
    <SimpleButton config />
  </View>;
};

let init = app => {
  // Revery.App.initConsole();

  // Timber.App.enable();
  // Timber.App.setLevel(Timber.Level.perf);

  let win =
    App.createWindow(
      app,
      "Welcome to Revery!",
      ~createOptions={
        ...Revery_Core.WindowCreateOptions.default,
        backgroundColor: Color.hex("#1f242e"),
        width: 1000,
        height: 2000,
      },
    );

  Printf.printf("Starting Event Loop\n%!");
  let _ignore = Revery_Lwt.startEventLoop();
  let _ignore = UI.start(win, <main />);
  ();
};

App.start(init);
