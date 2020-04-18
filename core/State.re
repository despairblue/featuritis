open Base;
open Stdio;
open FeatureFlags;
open Lib;

type t = {
  apiUrl: string,
  authToken: option(string),
  debug: bool,
  featureFlags: Request.t(array(featureFlags)),
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
    (~apiUrl="https://en-master.wunderflats.xyz/api/graphql", ~authToken=None) => {
  {
    apiUrl,
    authToken,
    debug: false,
    featureFlags: Idle,
    graphQLConfig:
      Option.map(authToken, ~f=authToken =>
        makeGraphqlConfig(~baseUrl=apiUrl, ~authToken)
      ),
  };
};

type action =
  | ChangeApiUrl(string)
  | DisableFeatureFlag(action => unit, string)
  | EnableFeatureFlag(action => unit, string)
  | LoadFeatureFlags(action => unit)
  | Login(string)
  | ToggleDebug
  | UpdateFeatureFlags(Request.t(array(featureFlags)));

let stringOfAction =
  fun
  | ChangeApiUrl(url) => "ChangeApiUrl: " ++ url
  | DisableFeatureFlag(_) => "DisableFeatureFlag"
  | EnableFeatureFlag(_) => "EnableFeatureFlag"
  | LoadFeatureFlags(_) => "LoadFeatureFlags"
  | Login(_) => "Login"
  | ToggleDebug => "ToggleDebug"
  | UpdateFeatureFlags(requestState) =>
    "UpdateFeatureFlags: " ++ Request.stringOfRequestState(requestState);

let reducer = (action, state) => {
  printf("Action: %s \n%!", stringOfAction(action));

  let newState =
    switch (action) {
    | ChangeApiUrl(apiUrl) =>
      Config.writeConfig({apiUrl, authToken: state.authToken});
      {...state, apiUrl};

    | UpdateFeatureFlags(featureFlags) => {...state, featureFlags}
    | LoadFeatureFlags(dispatch) =>
      open Lwt.Infix;

      // Basically the equivalent of nextTick and necessary to be able to dispatch again.
      Lwt.pause()
      >>= (
        () => {
          switch (state.authToken) {
          | None =>
            dispatch(UpdateFeatureFlags(Error("Not Authenticated")));
            Lwt.return_none;
          | Some(authToken) =>
            let query = FeatureFlags.Query.make(~type_=`ALL, ~state=`ALL, ());
            HTTP.sendQuery(
              ~apiUrl=state.apiUrl,
              ~query=query#query,
              ~variables=query#variables,
              ~authToken,
            );
          };
        }
      )
      >|= (
        fun
        | Some(data) => {
            let data = FeatureFlags.Query.parse(data);
            let featureFlags = data#featureFlags;

            dispatch(UpdateFeatureFlags(Data(featureFlags)));
          }
        | None =>
          dispatch(UpdateFeatureFlags(Error("Something went wrong")))
      )
      |> ignore;
      {...state, featureFlags: Loading};

    | EnableFeatureFlag(dispatch, name) =>
      open Lwt.Infix;

      Lwt.pause()
      >>= (
        () => {
          switch (state.authToken) {
          | None =>
            dispatch(UpdateFeatureFlags(Error("Not Authenticated")));
            Lwt.return_none;
          | Some(authToken) =>
            let mutation =
              FeatureFlags.EnableFeatureFlag.make(
                ~type_=`ALL,
                ~state=`ALL,
                ~name=[|name|],
                (),
              );

            HTTP.sendQuery(
              ~apiUrl=state.apiUrl,
              ~query=mutation#query,
              ~variables=mutation#variables,
              ~authToken,
            );
          };
        }
      )
      >|= (
        fun
        | Some(data) => {
            let data = FeatureFlags.EnableFeatureFlag.parse(data);
            let activateGlobalFeatureFlags = data#activateGlobalFeatureFlags;

            switch (activateGlobalFeatureFlags) {
            | `ActivateGlobalFeatureFlagsPayloadError(data) =>
              printf("Mutation failed: %s%!", data#message)
            | `ActivateGlobalFeatureFlagsPayloadSuccess(data) =>
              let featureFlags = data#featureFlags;
              dispatch(UpdateFeatureFlags(Data(featureFlags)));
            };
          }
        | None =>
          dispatch(UpdateFeatureFlags(Error("Something went wrong")))
      )
      |> ignore;

      let featureFlags =
        Request.map(state.featureFlags, ~f=featureFlags => {
          Array.map(featureFlags, ~f=ff =>
            if (phys_equal(ff.name, name)) {
              {...ff, enabled: true};
            } else {
              ff;
            }
          )
        });
      {...state, featureFlags};

    | DisableFeatureFlag(dispatch, name) =>
      open Lwt.Infix;

      Lwt.pause()
      >>= (
        () => {
          switch (state.authToken) {
          | None =>
            dispatch(UpdateFeatureFlags(Error("Not Authenticated")));
            Lwt.return_none;
          | Some(authToken) =>
            let mutation =
              FeatureFlags.DisableFeatureFlag.make(
                ~type_=`ALL,
                ~state=`ALL,
                ~name=[|name|],
                (),
              );
            HTTP.sendQuery(
              ~apiUrl=state.apiUrl,
              ~query=mutation#query,
              ~variables=mutation#variables,
              ~authToken,
            );
          };
        }
      )
      >|= (
        fun
        | Some(data) => {
            let data = FeatureFlags.DisableFeatureFlag.parse(data);
            let activateGlobalFeatureFlags = data#deactivateGlobalFeatureFlags;

            switch (activateGlobalFeatureFlags) {
            | `DeactivateGlobalFeatureFlagsPayloadError(data) =>
              printf("Mutation failed: %s%!", data#message)
            | `DeactivateGlobalFeatureFlagsPayloadSuccess(data) =>
              let featureFlags = data#featureFlags;
              dispatch(UpdateFeatureFlags(Data(featureFlags)));
            };
          }
        | None =>
          dispatch(UpdateFeatureFlags(Error("Something went wrong")))
      )
      |> ignore;

      let featureFlags =
        Request.map(state.featureFlags, ~f=featureFlags => {
          Array.map(featureFlags, ~f=ff =>
            if (phys_equal(ff.name, name)) {
              {...ff, enabled: false};
            } else {
              ff;
            }
          )
        });
      {...state, featureFlags};

    | ToggleDebug => {...state, debug: state.debug ? false : true}

    | Login(authToken) =>
      Config.writeConfig({apiUrl: state.apiUrl, authToken: Some(authToken)});
      {...state, authToken: Some(authToken)};
    };

  newState |> addGraphqlConfig;
};
