open Base;
open Stdio;
open FeatureFlags;

type t = {
  apiUrl: string,
  authToken: option(string),
  featureFlags: array(featureFlags),
};

let make =
    (~apiUrl="https://en-master.wunderflats.xyz/api/graphql", ~authToken=None) => {
  apiUrl,
  authToken,
  featureFlags: [||],
};

type action =
  | ChangeApiUrl(string)
  | ReplaceFeatureFlags(array(featureFlags))
  | EnableFeatureFlag(string)
  | DisableFeatureFlag(string)
  | Login(string);

let stringOfAction =
  fun
  | ChangeApiUrl(url) => "ChangeApiUrl: " ++ url
  | ReplaceFeatureFlags(_) => "ReplaceFeatureFlags"
  | EnableFeatureFlag(_) => "EnableFeatureFlag"
  | DisableFeatureFlag(_) => "DisableFeatureFlag"
  | Login(_) => "Login";

let reducer = (action, state) => {
  printf("Action: %s \n%!", stringOfAction(action));

  let newState =
    switch (action) {
    | ChangeApiUrl(apiUrl) => {...state, apiUrl}
    | ReplaceFeatureFlags(ffs) => {...state, featureFlags: ffs}
    | EnableFeatureFlag(name) =>
      let featureFlags =
        Array.map(state.featureFlags, ~f=ff =>
          if (phys_equal(ff.name, name)) {
            {...ff, enabled: true};
          } else {
            ff;
          }
        );

      {...state, featureFlags};
    | DisableFeatureFlag(name) =>
      let featureFlags =
        Array.map(state.featureFlags, ~f=ff =>
          if (phys_equal(ff.name, name)) {
            {...ff, enabled: false};
          } else {
            ff;
          }
        );

      {...state, featureFlags};
    | Login(authToken) => {...state, authToken: Some(authToken)}
    };

  Config.writeConfig({
    apiUrl: newState.apiUrl,
    authToken: newState.authToken,
  });

  newState;
};
