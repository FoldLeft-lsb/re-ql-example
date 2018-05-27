let str = ReasonReact.string;

let githubKey = [%bs.raw {|require('../config').github_key|}];

module ModuleQuery = [%graphql
  {|
    query($number_of_repos:Int!) {
      viewer {
        name
         repositories(last: $number_of_repos) {
           nodes {
             name
           }
         }
       }
    }
|}
];

let makeConnection = (~auth_code: string) : ApolloClient.generatedApolloClient =>
  ReasonApollo.createApolloClient(
    ~link=
      ApolloLinks.createHttpLink(
        ~uri="https://api.github.com/graphql",
        ~headers=
          Json.Encode.object_([
            ("Authorization", Js.Json.string("bearer " ++ auth_code)),
            ("Accept", Js.Json.string("application/vnd.github.v3+json")),
          ]),
        (),
      ),
    ~cache=ApolloInMemoryCache.createInMemoryCache(),
    (),
  );

type state_t = {apolloClient: ApolloClient.generatedApolloClient};

type action_t =
  | NoUpdate;

let component = ReasonReact.reducerComponent("ReqlQuery");

let make = _children => {
  ...component,
  initialState: () => {apolloClient: makeConnection(~auth_code=githubKey)},
  didMount: self =>
    Reql.query(
      self.state.apolloClient,
      ModuleQuery.make(~number_of_repos=3, ()) |> Reql.prepareQuery,
    )
    |> Js.Promise.then_(res => {
         Js.log("Re didMount: ");
         Js.log(res##data##viewer##name);
         /* self.send(DataLoaded(parsed(data))); */
         Js.Promise.resolve();
       })
    |> ignore,
  reducer: (action, _state) =>
    switch (action) {
    | NoUpdate => ReasonReact.NoUpdate
    },
  render: _self => <div> <h1> (str("Re Component")) </h1> </div>,
};