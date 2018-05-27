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
      ApolloLinks.createHttpLink(
        ~uri="https://q80vw8qjp.lp.gql.zone/graphql",
        (),
      ),
    ~cache=ApolloInMemoryCache.createInMemoryCache(),
    (),
  );

let githubAuthHeaders = {"Authorization": ""};

type state_t = {apolloClient: ApolloClient.generatedApolloClient};

type action_t =
  | NoUpdate;

let component = ReasonReact.reducerComponent("ReqlQuery");

let make = _children => {
  ...component,
  initialState: () => {apolloClient: makeConnection()},
  didMount: self =>
    Reql.query(self.state.apolloClient, moduleQuery)
    |> Js.Promise.then_(res => {
         Js.log("Re didMount: ");
         Js.log(res##data);
         Js.Promise.resolve();
       })
    |> ignore,
  reducer: (action, _state) =>
    switch (action) {
    | NoUpdate => ReasonReact.NoUpdate
    },
  render: _self => <div> <h1> (str("Re Component")) </h1> </div>,
};