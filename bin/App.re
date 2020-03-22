open Revery;
open Revery.UI;
open MyComponents.Components;

let main = () => {
  module Styles = {
    open Style;

    let container = [
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

    let inner = [flexDirection(`Row), alignItems(`FlexEnd)];
  };

  <View style=Styles.container>
    <View style=Styles.inner>
      <AnimatedText delay={Time.ms(0)} text="Welcome" />
      <AnimatedText delay={Time.ms(500)} text="to" />
      <AnimatedText delay={Time.ms(1000)} text="Featuritis" />
    </View>
    <SimpleButton />
  </View>;
};

let init = app => {
  // Revery.App.initConsole();

  // Timber.App.enable();
  // Timber.App.setLevel(Timber.Level.perf);

  let win = App.createWindow(app, "Welcome to Revery!");

  let _ignore = Revery_Lwt.startEventLoop();
  let _ignore = UI.start(win, <main />);
  ();
};

App.start(init);
