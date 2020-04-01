open Base;
open Stdio;
open FeatureFlags;
open Lib;

type t = {
  apiUrl: string,
  authToken: option(string),
  debug: bool,
  featureFlags: array(featureFlags),
  graphQLConfig: option(ReveryGraphqlHooks.config),
};

let makeGraphqlConfig = (~baseUrl, ~authToken): ReveryGraphqlHooks.config => {
  baseUrl,
  headers: [("authToken", authToken)],
};

let addGraphqlConfig = (state: t): t => {
  {
    ...state,
    graphQLConfig:
      Option.map(state.authToken, ~f=authToken =>
        makeGraphqlConfig(~baseUrl=state.apiUrl, ~authToken)
      ),
  };
};

let make =
    (
      ~apiUrl="https://en-master.wunderflats.xyz/api/graphql",
      ~authToken=None,
      (),
    ) => {
  {
    apiUrl,
    authToken,
    debug: false,
    featureFlags: [||],
    graphQLConfig:
      Option.map(authToken, ~f=authToken =>
        makeGraphqlConfig(~baseUrl=apiUrl, ~authToken)
      ),
  };
};

type action =
  | ChangeApiUrl(string)
  | DisableFeatureFlag(string)
  | EnableFeatureFlag(string)
  | Login(string)
  | Logout
  | ToggleDebug
  | UpdateFeatureFlags(array(featureFlags));

let stringOfAction =
  fun
  | ChangeApiUrl(url) => "ChangeApiUrl: " ++ url
  | DisableFeatureFlag(flag) =>
    Printf.sprintf({|DisableFeatureFlag "%s"|}, flag)
  | EnableFeatureFlag(flag) =>
    Printf.sprintf({|EnableFeatureFlag "%s"|}, flag)
  | Login(_authToken) => "Login"
  | Logout => "Logout"
  | ToggleDebug => "ToggleDebug"
  | UpdateFeatureFlags(_featureFlags) => "UpdateFeatureFlags";

let reducer = (action, state) => {
  printf("Action: %s \n%!", stringOfAction(action));

  let newState =
    switch (action) {
    | ChangeApiUrl(apiUrl) =>
      Config.writeConfig({apiUrl, authToken: state.authToken});
      {...state, apiUrl};

    | UpdateFeatureFlags(featureFlags) => {...state, featureFlags}

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

    | ToggleDebug => {...state, debug: state.debug ? false : true}

    | Login(authToken) =>
      Config.writeConfig({apiUrl: state.apiUrl, authToken: Some(authToken)});
      {...state, authToken: Some(authToken)};

    | Logout =>
      Config.writeConfig({apiUrl: state.apiUrl, authToken: None});
      {...state, authToken: None};
    };

  newState |> addGraphqlConfig;
};
