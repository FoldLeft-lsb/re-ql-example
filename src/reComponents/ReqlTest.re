let str = ReasonReact.string;

module ModuleQuery = [%graphql {|
  {
   hello
 }
|}];

let moduleQuery = ModuleQuery.make() |> Reql.prepareQuery;

let makeConnection = () : ApolloClient.generatedApolloClient =>
  ReasonApollo.createApolloClient(
    ~link=
      ApolloLinks.createHttpLink(~uri="https://api.github.com/graphql", ()),
    ~cache=ApolloInMemoryCache.createInMemoryCache(),
    (),
  );

let githubAuthHeaders = {"Authorization": ""};

type state_t = {client: ApolloClient.generatedApolloClient};

type action_t =
  | NoUpdate;

let component = ReasonReact.reducerComponent("ReqlTest");

let make = _children => {
  ...component,
  initialState: () => {client: makeConnection()},
  didMount: self =>
    Reql.query(self.state.client, moduleQuery)
    |> Js.Promise.then_(res => {
         Js.log("Re didMount: ");
         Js.log(res);
         Js.Promise.resolve();
       })
    |> ignore,
  reducer: (action, _state) =>
    switch (action) {
    | NoUpdate => ReasonReact.NoUpdate
    },
  render: _self => <div> <h1> (str("Re Component")) </h1> </div>,
};